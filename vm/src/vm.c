//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005 by Till Harbaum <Till@Harbaum.org>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// 

//
//  vm.c
//

#include <string.h>

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "vm.h"
#include "opcodes.h"
#include "native_impl.h"
#include "native.h"
#include "heap.h"
#include "nvmfile.h"
#include "stack.h"
#include "nvmfeatures.h"

#ifdef NVM_USE_ARRAY
#include "array.h"
#endif

#ifdef NVM_USE_32BIT_WORD
# define DBG_INT "0x" DBG32
#else
# define DBG_INT "0x" DBG16
#endif


void vm_init(void) {
  DEBUGF("vm_init() with %d static fields\n", nvmfile_get_static_fields());

  // init heap
  heap_init();

  // get stack space from heap and setup stack
  stack_init(nvmfile_get_static_fields());
 
  stack_push(0); // args parameter to main (should be a string array)
}

void *vm_get_addr(nvm_ref_t ref) {
  if(!(ref & NVM_IMMEDIATE_MASK))
    error(ERROR_VM_ILLEGAL_REFERENCE);

  if((ref & NVM_TYPE_MASK) == NVM_TYPE_HEAP)
    return heap_get_addr(ref & ~NVM_TYPE_MASK);

  // return nvmfile address and set marker indicating
  // that this is inside the nvm file (and may have
  // to be accessed in a special manner)
  return NVMFILE_SET(nvmfile_get_addr(ref & ~NVM_TYPE_MASK));
}

// expand 15 bit immediate to 16 bits (or 31 to 32)
nvm_int_t nvm_stack2int(nvm_stack_t val) {
  if(val & (NVM_IMMEDIATE_MASK>>1))
    val |= NVM_IMMEDIATE_MASK;   // expand sign bit
  return val;
}

#ifdef NVM_USE_FLOAT
nvm_stack_t nvm_float2stack(nvm_float_t val)
{
  nvm_union_t v;
  v.f[0]=val;
  //printf("float = %f == 0x%x", v.f[0], v.i[0]);
  uint8_t msb = (v.b[3]&0x80)?0x40:0x00;
  v.b[3] &= 0x7f;
  if (v.b[3]==0x7f && (v.b[2]&0x80)==0x80)
    msb |= 0x3f;
  else if (v.b[3]!=0x00 || (v.b[2]&0x80)!=0x00)
    msb |= v.b[3]-0x20;
  v.b[3]=msb;
  //printf(" -> encoded = 0x%x\n", v.i[0]);
  return v.i[0];
}

nvm_float_t nvm_stack2float(nvm_stack_t val)
{
  nvm_union_t v;
  v.i[0]=val;
  //printf("encoded = 0x%x", v.i[0]);
  uint8_t msb = (v.b[3]&0x40)?0x80:0x00;
  v.b[3] &= 0x3f;
  if (v.b[3]==0x3f && (v.b[2]&0x80)==0x80)
    msb |= 0x7f;
  else if (v.b[3]!=0x00 || (v.b[2]&0x80)!=0x00)
    msb |= v.b[3]+0x20;
  v.b[3]=msb;
  //printf(" -> float = %f == 0x%x\n", v.f[0], v.i[0]);
  return v.f[0];
}
#endif


nvm_stack_t *locals;

// pc/methodref/localsoffset
#define VM_METHOD_CALL_REQUIREMENTS 3

// create an instance of a class. check if it's local (within 
// the nvm file) or native (implemented by the runtime environment)
void vm_new(u16_t mref) {

  if(NATIVE_ID2CLASS(mref) < NATIVE_CLASS_BASE) {
    heap_id_t h;

    DEBUGF("local new #%d\n", NATIVE_ID2CLASS(mref));

    DEBUGF("non static fields: %d\n",
       nvmfile_get_class_fields(NATIVE_ID2CLASS(mref)));

    // create object with
    h = heap_alloc(TRUE, sizeof(nvm_word_t) *
     (VM_CLASS_CONST_ALLOC+nvmfile_get_class_fields(NATIVE_ID2CLASS(mref))));

    stack_push(NVM_TYPE_HEAP | h);

    // store reference in object, so we can later determine which kind
    // of object this is. this is required for inheritance
    ((nvm_ref_t*)heap_get_addr(h))[0] = mref;

    return;
  }

  native_new(mref);
}

// we prefetch arguments from the program storage
// and this is the type it is stored into

typedef union {
  s16_t w;
  struct {
    s08_t bl, bh;
  } z;
  nvm_int_t tmp;
} vm_arg_t;

void   vm_run(u16_t mref) {
  register u08_t instr, pc_inc, *pc;
  register nvm_int_t tmp1=0;
  register nvm_int_t tmp2;
  register vm_arg_t arg0;
  nvm_method_hdr_t mhdr, *mhdr_ptr;

#ifdef NVM_USE_FLOAT
  nvm_float_t f0;
  nvm_float_t f1;
#endif

#ifdef NVM_USE_STACK_CHECK
  stack_save_sp();
#endif

  DEBUGF("Running method %d\n", mref);

  // load method header into ram
  mhdr_ptr = nvmfile_get_method_hdr(mref);
  nvmfile_read(&mhdr, mhdr_ptr, sizeof(nvm_method_hdr_t));

  // determine method description address and code
  pc = (u08_t*)mhdr_ptr + mhdr.code_index;

  // make space for locals on the stack
  DEBUGF("Allocating space for %d local(s) and %d "
	     "stack elements - %d args\n", 
	     mhdr.max_locals, mhdr.max_stack, mhdr.args);
  
  // increase stack space. locals will be put on the stack as 
  // well. method arguments are part of the locals and are 
  // already on the stack
  heap_steal(sizeof(nvm_stack_t) * (mhdr.max_locals + mhdr.max_stack + mhdr.args));

  // determine address of current locals (stack pointer + 1)
  locals = stack_get_sp() + 1;
  stack_add_sp(mhdr.max_locals);
  stack_save_base();
  
  do {
    instr = nvmfile_read08(pc);
    pc_inc = 1;
    
    DEBUGF("%d/(sp:%d) - "DBG8" (%d): ", 
	       (pc-(u08_t*)mhdr_ptr) - mhdr.code_index, 
	       stack_get_depth(), instr, instr);
    
    // prefetch next args (in big endian order)
    arg0.z.bh = nvmfile_read08(pc+1);
    arg0.z.bl = nvmfile_read08(pc+2);

    if(instr == OP_NOP) {
      DEBUGF("nop\n");
    }
    
    else if(instr == OP_BIPUSH) {
      stack_push(arg0.z.bh); pc_inc = 2;
      DEBUGF("bipush #%d\n", stack_peek(0));
    } 

    else if(instr == OP_SIPUSH) {
      stack_push(~NVM_IMMEDIATE_MASK & (arg0.w)); pc_inc = 3;
      DEBUGF("sipush #"DBG16"\n", stack_peek_int(0));
    } 
    
    else if((instr >= OP_ICONST_M1) && (instr <= OP_ICONST_5)) {
      stack_push(instr - OP_ICONST_0);
      DEBUGF("iconst_%d\n", stack_peek(0));
    }   
    
    // move integer from stack into locals
    else if(instr == OP_ISTORE) {
      locals[arg0.z.bh] = stack_pop(); pc_inc = 2;
      DEBUGF("istore %d (%d)\n", arg0.z.bh, nvm_stack2int(locals[arg0.z.bh]));
    } 
    
    // move integer from stack into locals
    else if((instr >= OP_ISTORE_0) && (instr <= OP_ISTORE_3)) {
      locals[instr - OP_ISTORE_0] = stack_pop();
      DEBUGF("istore_%d (%d)\n", instr - OP_ISTORE_0, 
		 nvm_stack2int(locals[instr - OP_ISTORE_0]));
    } 

    // load int from local variable (push local var)
    else if(instr == OP_ILOAD) {
      stack_push(locals[arg0.z.bh]); pc_inc = 2;
      DEBUGF("iload %d (%d, "DBG_INT")\n", locals[arg0.z.bh],
		 stack_peek_int(0), stack_peek_int(0));
    } 

    // push local onto stack
    else if((instr >= OP_ILOAD_0) && (instr <= OP_ILOAD_3)) {
      stack_push(locals[instr - OP_ILOAD_0]);
      DEBUGF("iload_%d (%d, "DBG_INT")\n", instr-OP_ILOAD_0,
		 stack_peek_int(0), stack_peek_int(0));
    } 

    // immediate comparison / comparison with zero
    else if((instr >= OP_IFEQ) && (instr <= OP_IF_ICMPLE)) {
      DEBUGF("if");

      if((instr >= OP_IFEQ) && (instr <= OP_IFLE)) {
	// comparision with zero
	tmp2 = 0;
	instr -= OP_IFEQ - OP_IF_ICMPEQ;
      } else {
	// comparison with second argument
	DEBUGF("_cmp");
	tmp2 = stack_pop_int();
      }

      tmp1 = stack_pop_int();

      switch(instr) {
        case OP_IF_ICMPEQ: DEBUGF("eq (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 == tmp2); break;
        case OP_IF_ICMPNE: DEBUGF("ne (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 != tmp2); break;
        case OP_IF_ICMPLT: DEBUGF("lt (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 <  tmp2); break;
        case OP_IF_ICMPGE: DEBUGF("ge (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 >= tmp2); break;
        case OP_IF_ICMPGT: DEBUGF("gt (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 >  tmp2); break;
        case OP_IF_ICMPLE: DEBUGF("le (%d %d)", tmp1, tmp2);
          tmp1 = (tmp1 <= tmp2); break;
      }
      
      // change pc if jump has been taken
      if(tmp1) { DEBUGF(" -> taken\n"); pc += arg0.w; pc_inc = 0; }
      else     { DEBUGF(" -> not taken\n"); pc_inc = 3; }
    } 

    else if(instr == OP_GOTO) {
      pc_inc = 3;
      DEBUGF("goto %d\n", arg0.w); 
      pc += (arg0.w-3);
    } 

    // two operand arithmetic
    else if((instr >= OP_IADD) && (instr <= OP_IINC)) {
      // single operand arithmetic
      if(instr == OP_INEG) {
	tmp1 = -stack_pop_int();
	stack_push(nvm_int2stack(tmp1));
        DEBUGF("ineg(%d)\n", -stack_peek_int(0));

      } else if(instr == OP_IINC) {
	DEBUGF("iinc %d,%d\n", arg0.z.bh, arg0.z.bl); 
	locals[arg0.z.bh] = (nvm_stack2int(locals[arg0.z.bh]) + arg0.z.bl) 
	  & ~NVM_IMMEDIATE_MASK; 
	pc_inc = 3;

#ifdef NVM_USE_FLOAT
      } else if(((instr & 0x03) == 0x02) && (instr <= OP_FNEG)) {
        if (instr == OP_FNEG) {
          f0 = -stack_pop_float();
          stack_push(nvm_float2stack(f0));
          DEBUGF("fneg (%f)\n", stack_peek_float(0));
        }
        else {
          f0 = stack_pop_float();  // fetch operands from stack
          f1 = stack_pop_float();
          switch(instr) {
            case OP_FADD:  DEBUGF("fadd(%f,%f)", f1, f0);
              f1  += f0; break;
            case OP_FSUB:  DEBUGF("fsub(%f,%f)", f1, f0);
              f1  -= f0; break;
            case OP_FMUL:  DEBUGF("fmul(%f,%f)", f1, f0);
              f1  *= f0; break;
            case OP_FDIV:  DEBUGF("fdiv(%f,%f)", f1, f0);
              if(!f0) error(ERROR_VM_DIVISION_BY_ZERO);
              f1  /= f0; break;
            case OP_IREM:  DEBUGF("frem(%f,%f)", f1, f0);
              error(ERROR_VM_UNSUPPORTED_OPCODE);
              //f1  = f1%f0; break;
          }
          stack_push(nvm_float2stack(f1));
          DEBUGF(" = %f\n", stack_peek_float(0));
        }
#endif

      } else {
	tmp1 = stack_pop_int();  // fetch operands from stack
	tmp2 = stack_pop_int();
	
	switch(instr) {
          case OP_IADD:  DEBUGF("iadd(%d,%d)", tmp2, tmp1);
	    tmp2  += tmp1; break;
	  case OP_ISUB:  DEBUGF("isub(%d,%d)", tmp2, tmp1);
	    tmp2  -= tmp1; break;
	  case OP_IMUL:  DEBUGF("imul(%d,%d)", tmp2, tmp1);
	    tmp2  *= tmp1; break;
	  case OP_IDIV:  DEBUGF("idiv(%d,%d)", tmp2, tmp1);
	    if(!tmp1) error(ERROR_VM_DIVISION_BY_ZERO);
	    tmp2  /= tmp1; break;
	  case OP_IREM:  DEBUGF("irem(%d,%d)", tmp2, tmp1);
	    tmp2  %= tmp1; break;
	  case OP_ISHL:  DEBUGF("ishl(%d,%d)", tmp2, tmp1);
	    tmp2 <<= tmp1; break;
	  case OP_ISHR:  DEBUGF("ishr(%d,%d)", tmp2, tmp1);
	    tmp2 >>= tmp1; break;
	  case OP_IAND:  DEBUGF("iand(%d,%d)", tmp2, tmp1);
	    tmp2  &= tmp1; break;
	  case OP_IOR:   DEBUGF("ior(%d,%d)",  tmp2, tmp1);
	    tmp2  |= tmp1; break;
	  case OP_IXOR:  DEBUGF("ixor(%d,%d)", tmp2, tmp1);
	    tmp2  ^= tmp1; break;
	  case OP_IUSHR: DEBUGF("iushr(%d,%d)", tmp2, tmp1);
	    tmp2 = ((nvm_uint_t)tmp2 >> tmp1); break;
	}
	
	// and finally push result
        stack_push(nvm_int2stack(tmp2));
        DEBUGF(" = %d\n", stack_peek_int(0));
      }
    }

    else if((instr == OP_IRETURN)
#ifdef NVM_USE_FLOAT
          ||(instr == OP_FRETURN)
#endif
          ||(instr == OP_RETURN)) {
      if((instr == OP_IRETURN)
#ifdef NVM_USE_FLOAT
       ||(instr == OP_FRETURN)
#endif
      ) {
	tmp1 = stack_pop();     // save result
	DEBUGF("i");
      }

      DEBUGF("return: ");

      // return from locally called method? other case: return
      // from main() -> end of program
      if(!stack_is_empty()) {
	u08_t old_locals = mhdr.max_locals;
	u08_t old_unsteal = VM_METHOD_CALL_REQUIREMENTS +
	  mhdr.max_locals + mhdr.max_stack + mhdr.args;
	u16_t old_localsoffset = stack_pop();
	
	// make space for locals on the stack
	DEBUGF("Return from method with %d local(s) and %d "
		   "stack elements - %d args\n", 
		   mhdr.max_locals, mhdr.max_stack, mhdr.args);
	
	mref = stack_pop();
	
	// read header of method to return to
	mhdr_ptr = nvmfile_get_method_hdr(mref);
	// load method header into ram
	nvmfile_read(&mhdr, mhdr_ptr, sizeof(nvm_method_hdr_t));
	
	// restore pc
	pc = (u08_t*)mhdr_ptr + stack_pop();
	pc_inc = 3; // continue _behind_ calling invoke instruction
	
	// and remove locals from stack and hope that method left
	// an uncorrupted stack
	stack_add_sp(-old_locals);
	locals = stack_get_sp() - old_localsoffset;
	
	// give memory used by returning method back to heap
	heap_unsteal(sizeof(nvm_stack_t) * old_unsteal);
	
        if(instr == OP_IRETURN){
          stack_push(tmp1);
          DEBUGF("ireturn val: %d\n", stack_peek_int(0));
        }
#ifdef NVM_USE_FLOAT
        else if(instr == OP_FRETURN){
	  stack_push(tmp1);
          DEBUGF("freturn val: %f\n", stack_peek_float(0));
	}
#endif
	instr = OP_NOP;  // make vm continue
      }
    }

    // discard both top stack items
    else if(instr == OP_POP2) {
      DEBUGF("ipop\n");
      stack_pop(); stack_pop();
    }
    
    // discard top stack item
    else if(instr == OP_POP) {
      DEBUGF("pop\n");
      stack_pop();
    }
    
    // duplicate top stack item
    else if(instr == OP_DUP) {
      stack_push(stack_peek(0));
      DEBUGF("dup ("DBG16")\n", stack_peek(0) & 0xffff);
    }

    // duplicate top two stack items  (a,b -> a,b,a,b)
    else if(instr == OP_DUP2) {
      stack_push(stack_peek(1));
      stack_push(stack_peek(1));
      DEBUGF("dup2 ("DBG16","DBG16")\n", 
	     stack_peek(0) & 0xffff, stack_peek(1) & 0xffff);
    }

#ifdef NVM_USE_EXTSTACKOPS
    
    // duplicate top stack item and put it under the second
    else if(instr == OP_DUP_X1) {
      nvm_stack_t w1 = stack_pop();
      nvm_stack_t w2 = stack_pop();
      stack_push(w1);
      stack_push(w2);
      stack_push(w1);
      DEBUGF("dup_x1 ("DBG16")\n", stack_peek(0) & 0xffff);
    }

    // duplicate top stack item
    else if(instr == OP_DUP_X2) {
      nvm_stack_t w1 = stack_pop();
      nvm_stack_t w2 = stack_pop();
      nvm_stack_t w3 = stack_pop();
      stack_push(w1);
      stack_push(w2);
      stack_push(w3);
      stack_push(w1);
      DEBUGF("dup ("DBG16")\n", stack_peek(0) & 0xffff);
    }

    // duplicate top two stack items  (a,b -> a,b,a,b)
    else if(instr == OP_DUP2_X1) {
      nvm_stack_t w1 = stack_pop();
      nvm_stack_t w2 = stack_pop();
      nvm_stack_t w3 = stack_pop();
      stack_push(w1);
      stack_push(w2);
      stack_push(w3);
      stack_push(w1);
      stack_push(w2);
      DEBUGF("dup2 ("DBG16","DBG16")\n",
             stack_peek(0) & 0xffff, stack_peek(1) & 0xffff);
    }

    // duplicate top two stack items  (a,b -> a,b,a,b)
    else if(instr == OP_DUP2_X2) {
      nvm_stack_t w1 = stack_pop();
      nvm_stack_t w2 = stack_pop();
      nvm_stack_t w3 = stack_pop();
      nvm_stack_t w4 = stack_pop();
      stack_push(w1);
      stack_push(w2);
      stack_push(w3);
      stack_push(w4);
      stack_push(w1);
      stack_push(w2);
      DEBUGF("dup2 ("DBG16","DBG16")\n",
             stack_peek(0) & 0xffff, stack_peek(1) & 0xffff);
    }
    
    // swap top two stack items  (a,b -> b,a)
    else if(instr == OP_SWAP) {
      nvm_stack_t w1 = stack_pop();
      nvm_stack_t w2 = stack_pop();
      stack_push(w1);
      stack_push(w2);
      DEBUGF("swap ("DBG16","DBG16")\n", stack_peek(0), stack_peek(1));
    }
    
#endif
    
    
#ifdef NVM_USE_TABLESWITCH
    else if(instr == OP_TABLESWITCH) {
      DEBUGF("TABLESWITCH\n");
      // padding was eliminated by generator
      tmp1 = ((nvmfile_read08(pc+7)<<8) |
	      nvmfile_read08(pc+8));        // get low value
      tmp2 = ((nvmfile_read08(pc+11)<<8) |
	      nvmfile_read08(pc+12));       // get high value
      arg0.tmp = stack_pop();               // get actual value
      DEBUGF("tableswitch %d-%d (%d)\n", tmp1, tmp2, arg0.w);
      
      // value within range?
      if((arg0.tmp < tmp1)||(arg0.tmp > tmp2))
	// no: use default
	tmp2 = 3;
      else
	// yes: get offset from table
	tmp2 = 3 + 12 + ((arg0.tmp - tmp1)<<2);
      
      // and do the jump
      pc += ((nvmfile_read08(pc+tmp2+0)<<8) | 
	     nvmfile_read08(pc+tmp2+1));
      pc_inc = 0;
    }
#endif
    
#ifdef NVM_USE_LOOKUPSWITCH
    else if(instr == OP_LOOKUPSWITCH) {
      u08_t size;

      DEBUGF("LOOKUPSWITCH\n");
      // padding was eliminated by generator
     
      arg0.tmp = 1 + 4;
      size = nvmfile_read08(pc+arg0.tmp+3); // get table size (max for nvm is 30 cases!)
      DEBUGF("  size: %d\n", size);
      arg0.tmp += 4;
      
      tmp1 = stack_pop_int();                        // get actual value
      DEBUGF("  val=: %d\n", tmp1);
      
      while(size)
      {
        if (
#ifdef NVM_USE_32BIT_WORD
             nvmfile_read08(pc+arg0.tmp+0)==(u08_t)(tmp1>>24) &&
             nvmfile_read08(pc+arg0.tmp+1)==(u08_t)(tmp1>>16) &&
#endif
             nvmfile_read08(pc+arg0.tmp+2)==(u08_t)(tmp1>>8) &&
             nvmfile_read08(pc+arg0.tmp+3)==(u08_t)(tmp1>>0)
           )
        {
          DEBUGF("  value found, index is %d\n", (int)(arg0.tmp-pc_inc-8)/8);
          arg0.tmp+=4;
          break;
        }
        arg0.tmp+=8;
        size--;
      }
      
      if (size==0)
      {
        DEBUGF("  not found, using default!\n");
        arg0.tmp = 1;
      }
      pc += ((nvmfile_read08(pc+arg0.tmp+2)<<8) |
             nvmfile_read08(pc+arg0.tmp+3));
      pc_inc = 0;
    }
#endif

    // get static field from class
    else if(instr == OP_GETSTATIC) {
      pc_inc = 3;   // prefetched data used
      DEBUGF("getstatic #"DBG16"\n", arg0.w);
      stack_push(stack_get_static(arg0.w));
    }
    
    else if(instr == OP_PUTSTATIC) {
      pc_inc = 3;
      stack_set_static(arg0.w, stack_pop());
      DEBUGF("putstatic #"DBG16" -> "DBG16"\n", 
	     arg0.w, stack_get_static(arg0.w));
    }
    
    // push item from constant pool
    else if(instr == OP_LDC) {
      pc_inc = 2;
      DEBUGF("ldc #"DBG16"\n", arg0.z.bh);
#ifdef NVM_USE_32BIT_WORD
      stack_push(nvmfile_get_constant(arg0.z.bh));
#else
      stack_push(NVM_TYPE_CONST | (arg0.z.bh-nvmfile_constant_count));
#endif
    }
    
    else if((instr >= OP_INVOKEVIRTUAL)&&(instr <= OP_INVOKESTATIC)) {
      DEBUGF("invoke");

#ifdef DEBUG
      if(instr == OP_INVOKEVIRTUAL) { DEBUGF("virtual"); }
      if(instr == OP_INVOKESPECIAL) { DEBUGF("special"); }
      if(instr == OP_INVOKESTATIC)  { DEBUGF("static"); }
#endif

      DEBUGF(" #"DBG16"\n", 0xffff & arg0.w);
      
      // invoke a method. check if it's local (within the nvm file)
      // or native (implemented by the runtime environment)
      if(arg0.z.bh < NATIVE_CLASS_BASE) {
	DEBUGF("local method call from method %d to %d\n", mref, arg0.w);

	// save current pc (relative to method start)
	tmp1 = (u08_t*)pc-(u08_t*)mhdr_ptr;
	
	// get pointer to new method
	mhdr_ptr = nvmfile_get_method_hdr(arg0.w);
	
	// load new method header into ram
	nvmfile_read(&mhdr, mhdr_ptr, sizeof(nvm_method_hdr_t));
	
#ifdef NVM_USE_INHERITANCE
	// check class on stack. it may be not the one we expect.
	// this happens due to inheritance
	if(instr == OP_INVOKEVIRTUAL) { 
	  nvm_ref_t mref;

	  DEBUGF("checking inheritance\n");

	  // fetch class reference from stack and use it to address
	  // the class instance on the heap. The first entry in this 
	  // object is the class id of it
	  mref = ((nvm_ref_t*)heap_get_addr(stack_peek(0) & ~NVM_TYPE_MASK))[0];
	  DEBUGF("class ref on stack/ref: %d/%d\n", 
		     NATIVE_ID2CLASS(mref), NATIVE_ID2CLASS(mhdr.id));

	  if(NATIVE_ID2CLASS(mref) != NATIVE_ID2CLASS(mhdr.id)) {
	    DEBUGF("stack/ref class mismatch -> inheritance\n");

	    // get matching method in class on stack or its
	    // super classes
	    arg0.z.bl = nvmfile_get_method_by_class_and_id(
	      NATIVE_ID2CLASS(mref), NATIVE_ID2METHOD(mhdr.id));

	    // get pointer to new method
	    mhdr_ptr = nvmfile_get_method_hdr(arg0.z.bl);
	
	    // load new method header into ram
	    nvmfile_read(&mhdr, mhdr_ptr, sizeof(nvm_method_hdr_t));
	  }
	}
#endif
	
	// arguments are left on the stack by the calling
	// method and expected in the locals by the called
	// method. Thus we make this part of the old stack
	// be the locals part of the method
	DEBUGF("Remove %d args from stack\n", mhdr.args);
	stack_add_sp(-mhdr.args);
	
	tmp2 = stack_get_sp() - locals;
	
	locals = stack_get_sp() + 1;
	
#ifdef DEBUG
	if(instr == OP_INVOKEVIRTUAL) { 
	  DEBUGF("virtual call with object reference "DBG16"\n",
		     locals[0]); 
	}
#endif

	// make space for locals on the stack
	DEBUGF("Allocating space for %d local(s) and %d "
		   "stack elements - %d args\n", 
		   mhdr.max_locals, mhdr.max_stack, mhdr.args);
	
	// increase stack space. locals will be put on the stack as 
	// well. method arguments are part of the locals and are 
	// already on the stack
	heap_steal(sizeof(nvm_stack_t) *
		   (VM_METHOD_CALL_REQUIREMENTS +
		    mhdr.max_locals + mhdr.max_stack + mhdr.args));
	
	// add space for locals on stack
	stack_add_sp(mhdr.max_locals);
	
	// push everything required to return onto the stack
	stack_push(tmp1);   // pc offset
	stack_push(mref);   // method reference
	stack_push(tmp2);   // locals offset
	
	// set new pc (this is the actual call)
	mref = arg0.w;
	pc = (u08_t*)mhdr_ptr + mhdr.code_index;
	pc_inc = 0;  // don't add further bytes to program counter
      } else { 
	native_invoke(arg0.w);
	pc_inc = 3;   // prefetched data used
      }
    }
    
    else if(instr == OP_GETFIELD) {
      pc_inc = 3;
      DEBUGF("getfield #%d\n", arg0.w);
      stack_push(((nvm_word_t*)heap_get_addr(stack_pop() & ~NVM_TYPE_MASK))
	      [VM_CLASS_CONST_ALLOC+arg0.w]);
    }
    
    else if(instr == OP_PUTFIELD) {
      pc_inc = 3;
      tmp1 = stack_pop();
      
      DEBUGF("putfield #%d\n", arg0.w);
      ((nvm_word_t*)heap_get_addr(stack_pop() & ~NVM_TYPE_MASK))
	[VM_CLASS_CONST_ALLOC+arg0.w] = tmp1;
    }
    
    else if(instr == OP_NEW) {
      pc_inc = 3;
      DEBUGF("new #"DBG16"\n", 0xffff & arg0.w);
      vm_new(arg0.w);
    }
    
#ifdef NVM_USE_ARRAY
    else if(instr == OP_NEWARRAY) {
      pc_inc = 2;
      stack_push(array_new(stack_pop(), arg0.z.bh) | NVM_TYPE_HEAP);
    }
    
    else if(instr == OP_ARRAYLENGTH) {
      stack_push(array_length(stack_pop() & ~NVM_TYPE_MASK));
    }
    
    else if(instr == OP_BASTORE) {
      tmp2 = stack_pop_int();       // value
      tmp1 = stack_pop_int();         // index
      // third parm on stack: array reference
      array_bastore(stack_pop() & ~NVM_TYPE_MASK, tmp1, tmp2);
    }
    
    else if(instr == OP_IASTORE) {
      tmp2 = stack_pop_int();       // value
      tmp1 = stack_pop_int();       // index
      // third parm on stack: array reference
      array_iastore(stack_pop() & ~NVM_TYPE_MASK, tmp1, tmp2);
    }
    
    else if(instr == OP_BALOAD) {
      tmp1 = stack_pop_int();       // index
      // second parm on stack: array reference
      stack_push(array_baload(stack_pop() & ~NVM_TYPE_MASK, tmp1));
    }
    
    else if(instr == OP_IALOAD) {
      tmp1 = stack_pop_int();       // index
      // second parm on stack: array reference
      stack_push(array_iaload(stack_pop() & ~NVM_TYPE_MASK, tmp1));
    }
#endif

#ifdef NVM_USE_OBJ_ARRAY
    else if(instr == OP_ANEWARRAY) {
      // Object array is the same as int array...
      pc_inc = 3;
      stack_push(array_new(stack_pop(), T_INT) | NVM_TYPE_HEAP);
    }
    
    else if(instr == OP_AASTORE) {
      tmp2 = stack_pop_int();       // value
      tmp1 = stack_pop_int();       // index
      // third parm on stack: array reference
      array_iastore(stack_pop(), tmp1, tmp2);
    }
    
    else if(instr == OP_AALOAD) {
      tmp1 = stack_pop_int();       // index
      // second parm on stack: array reference
      stack_push(array_iaload(stack_pop(), tmp1));
    }
#endif

#ifdef NVM_USE_FLOAT
# ifdef NVM_USE_ARRAY
    else if(instr == OP_FALOAD) {
      tmp1 = stack_pop_int();       // index
      // second parm on stack: array reference
      stack_push(array_faload(stack_pop() & ~NVM_TYPE_MASK, tmp1));
    }
    else if(instr == OP_FASTORE) {
      f0 = stack_pop_float();       // value
      tmp1 = stack_pop_int();         // index
      // third parm on stack: array reference
      array_fastore(stack_pop() & ~NVM_TYPE_MASK, tmp1, f0);
    }
# endif

    else if(instr == OP_FCONST_0) {
      stack_push(nvm_float2stack(0.0));
      DEBUGF("fconst_%d\n", stack_peek_float(0));
    }
    else if(instr == OP_FCONST_1) {
      stack_push(nvm_float2stack(1.0));
      DEBUGF("fconst_%d\n", stack_peek_float(0));
    }
    else if(instr == OP_FCONST_2) {
      stack_push(nvm_float2stack(2.0));
      DEBUGF("fconst_%d\n", stack_peek_float(0));
    }
    else if(instr == OP_I2F) {
      tmp1 = stack_pop_int();
      stack_push(nvm_float2stack(tmp1));
      DEBUGF("i2f %f\n", stack_peek_float(0));
    }
    else if(instr == OP_F2I) {
      tmp1 = stack_pop_float();
      stack_push(nvm_int2stack(tmp1));
      DEBUGF("i2f %f\n", stack_peek_int(0));
    }
    
    // move float from stack into locals
    else if(instr == OP_FSTORE) {
      locals[arg0.z.bh] = stack_pop(); pc_inc = 2;
      DEBUGF("fstore %d (%f)\n", arg0.z.bh, nvm_stack2float(locals[arg0.z.bh]));
    } 
    
    // move integer from stack into locals
    else if((instr >= OP_FSTORE_0) && (instr <= OP_FSTORE_3)) {
      locals[instr - OP_FSTORE_0] = stack_pop();
      DEBUGF("fstore_%d (%f)\n", instr - OP_FSTORE_0,
      nvm_stack2float(locals[instr - OP_FSTORE_0]));
    } 

    // load float from local variable (push local var)
    else if(instr == OP_FLOAD) {
      stack_push(locals[arg0.z.bh]); pc_inc = 2;
      DEBUGF("fload %d (%f, "DBG16")\n", locals[arg0.z.bh],
      stack_peek_float(0), stack_peek_int(0));
    } 

    // push local onto stack
    else if((instr >= OP_FLOAD_0) && (instr <= OP_FLOAD_3)) {
      stack_push(locals[instr - OP_FLOAD_0]);
      DEBUGF("fload_%d (%f, "DBG16")\n", instr-OP_FLOAD_0,
      stack_peek_float(0), stack_peek_int(0));
    }
    
    // compare top values on stack
    else if((instr == OP_FCMPL) || (instr == OP_FCMPG)) {
      f1 = stack_pop_float();
      f0 = stack_pop_float();
      tmp1=0;
      if (f0<f1)
        tmp1=-1;
      else if (f0>f1)
        tmp1=1;
      stack_push(nvm_int2stack(tmp1));
      DEBUGF("fcmp%c (%f, %f, %i)\n", (instr==OP_FCMPL)?'l':'g',
      f0, f1, stack_peek_int(0));
    }
#endif

    else {
      error(ERROR_VM_UNSUPPORTED_OPCODE);
    }
    
    // reset watchdog here if present

    pc += pc_inc;
  } while((instr != OP_IRETURN)&&(instr != OP_RETURN));

  // and remove locals from stack and hope that method left
  // an uncorrupted stack
  stack_add_sp(-mhdr.max_locals);

#ifdef NVM_USE_STACK_CHECK
  stack_verify_sp();
#endif

  // give memory back to heap
  heap_unsteal(sizeof(nvm_stack_t) * (mhdr.max_locals + mhdr.max_stack + mhdr.args));
}

