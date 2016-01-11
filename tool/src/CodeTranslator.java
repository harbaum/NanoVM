//
//  NanoVMTool, Converter and Upload Tool for the NanoVM
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
//  Parts of this tool are based on public domain code written by Kimberley
//  Burchett: http://www.kimbly.com/code/classfile/
//

public class CodeTranslator {

  // parameter bytes for each of the 256 instructions (-1 = not implemented)
  final static int[] PARAMETER_BYTES = {
 // 00  01  02  03  04  05  06  07  08  09  0a  0b  0c  0d  0e  0f
     0, -1,  0,  0,  0,  0,  0,  0,  0, -1, -1,  0,  0,  0, -1, -1, // 00
     1,  2,  1, -1, -1,  1, -1,  1, -1,  1,  0,  0,  0,  0, -1, -1, // 10
    -1, -1,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0,  0,  0,  0, -1, // 20
     0, -1,  0,  0, -1, -1,  1, -1,  1, -1, -1,  0,  0,  0,  0, -1, // 30

    -1, -1, -1,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0,  0,  0,  0, // 40
    -1,  0, -1,  0,  0, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 50
     0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, // 60
     0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1,  0, -1, // 70

     0, -1,  0, -1,  2, -1,  0, -1, -1, -1, -1,  0, -1, -1, -1, -1, // 80
    -1, -1,  0, -1, -1,  0,  0, -1, -1,  2,  2,  2,  2,  2,  2,  2, // 90
     2,  2,  2,  2,  2, -1, -1,  2, -1, -1,  0,  0,  0, -1,  0, -1, // a0
    -1,  0,  2,  2,  2,  2,  2,  2,  2, -1, -1,  2,  1,  2,  0, -1, // b0

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // c0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // d0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // e0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // f0
  };

  // some java bytecode instructions
  final static int OP_NOP           = 0x00;
  final static int OP_ACONST_NULL   = 0x01;
  final static int OP_ICONST_0      = 0x03;
  final static int OP_SIPUSH        = 0x11;
  final static int OP_LDC           = 0x12;
  final static int OP_ILOAD         = 0x15;
  final static int OP_ALOAD         = 0x19;
  final static int OP_ILOAD_0       = 0x1a;
  final static int OP_ILOAD_1       = 0x1b;
  final static int OP_ILOAD_2       = 0x1c;
  final static int OP_ILOAD_3       = 0x1d;
  final static int OP_ALOAD_0       = 0x2a;
  final static int OP_ALOAD_1       = 0x2b;
  final static int OP_ALOAD_2       = 0x2c;
  final static int OP_ALOAD_3       = 0x2d;
  final static int OP_ISTORE        = 0x36;
  final static int OP_ASTORE        = 0x3a;
  final static int OP_ISTORE_0      = 0x3b;
  final static int OP_ISTORE_1      = 0x3c;
  final static int OP_ISTORE_2      = 0x3d;
  final static int OP_ISTORE_3      = 0x3e;
  final static int OP_ASTORE_0      = 0x4b;
  final static int OP_ASTORE_1      = 0x4c;
  final static int OP_ASTORE_2      = 0x4d;
  final static int OP_ASTORE_3      = 0x4e;
  final static int OP_I2B           = 0x91;
  final static int OP_I2C           = 0x92;
  final static int OP_I2S           = 0x93;
  final static int OP_TABLESWITCH   = 0xaa;
  final static int OP_LOOKUPSWITCH  = 0xab;
  final static int OP_IRETURN       = 0xac;
  final static int OP_ARETURN       = 0xb0;

  final static int OP_GETSTATIC     = 0xb2;
  final static int OP_PUTSTATIC     = 0xb3;
  final static int OP_GETFIELD      = 0xb4;
  final static int OP_PUTFIELD      = 0xb5;
  final static int OP_INVOKEVIRTUAL = 0xb6;
  final static int OP_INVOKESPECIAL = 0xb7;
  final static int OP_INVOKESTATIC  = 0xb8;
  final static int OP_NEW           = 0xbb;
  
  final static int OP_IFEQ           = 0x99;
  final static int OP_IFNE           = 0x9a;
  final static int OP_IFNULL         = 0xc6;
  final static int OP_IFNONNULL      = 0xc7;

  final static int  OP_FCONST_0     = 0x0b; // only if floating point compiled in
  final static int  OP_FCONST_1     = 0x0c; // only if floating point compiled in
  final static int  OP_FCONST_2     = 0x0d; // only if floating point compiled in
  final static int  OP_FLOAD        = 0x17; // only if floating point compiled in
  final static int  OP_FLOAD_0      = 0x22; // only if floating point compiled in
  final static int  OP_FLOAD_1      = 0x23; // only if floating point compiled in
  final static int  OP_FLOAD_2      = 0x24; // only if floating point compiled in
  final static int  OP_FLOAD_3      = 0x25; // only if floating point compiled in
  final static int  OP_IALOAD       = 0x2e; // only if array compiled in
  final static int  OP_FALOAD       = 0x30; // only if array and floating point compiled in
  final static int  OP_AALOAD       = 0x32; // only if array compiled in
  final static int  OP_BALOAD       = 0x33; // only if array compiled in
  final static int  OP_FSTORE       = 0x38; // only if floating point compiled in
  final static int  OP_FSTORE_0     = 0x43; // only if floating point compiled in
  final static int  OP_FSTORE_1     = 0x44; // only if floating point compiled in
  final static int  OP_FSTORE_2     = 0x45; // only if floating point compiled in
  final static int  OP_FSTORE_3     = 0x46; // only if floating point compiled in
  final static int  OP_IASTORE      = 0x4f; // only if array compiled in
  final static int  OP_FASTORE      = 0x51; // only if array and floating point compiled in
  final static int  OP_AASTORE      = 0x53; // only if array compiled in
  final static int  OP_BASTORE      = 0x54; // only if array compiled in
  final static int  OP_DUP_X1       = 0x5a; // only if extended stack ops compiled in
  final static int  OP_DUP_X2       = 0x5b; // only if extended stack ops compiled in
  final static int  OP_DUP2_X1      = 0x5d; // only if extended stack ops compiled in
  final static int  OP_DUP2_X2      = 0x5e; // only if extended stack ops compiled in
  final static int  OP_SWAP         = 0x5f; // only if extended stack ops compiled in
  final static int  OP_FADD         = 0x62; // only if floating point compiled in
  final static int  OP_FSUB         = 0x66; // only if floating point compiled in
  final static int  OP_FMUL         = 0x6a; // only if floating point compiled in
  final static int  OP_FDIV         = 0x6e; // only if floating point compiled in
  final static int  OP_FREM         = 0x72; // only if floating point compiled in
  final static int  OP_FNEG         = 0x76; // only if floating point compiled in
  final static int  OP_I2F          = 0x86; // only if floating point compiled in
  final static int  OP_F2I          = 0x8b; // only if floating point compiled in
  final static int  OP_FCMPL        = 0x95; // only if floating point compiled in
  final static int  OP_FCMPG        = 0x96; // only if floating point compiled in
  final static int  OP_FRETURN      = 0xae; // only if floating point compiled in
  final static int  OP_NEWARRAY     = 0xbc; // only if array compiled in
  final static int  OP_ANEWARRAY    = 0xbd; // only if array compiled in
  final static int  OP_ARRAYLENGTH  = 0xbe; // only if array compiled in


  
  static int unsigned(int i) {
    if(i<0) return i + 256;
    return i;
  }

  static byte signed(int i) {
    if(i>127) return (byte)(i - 256);
    return (byte)i;
  }

  static int get32(byte[] code, int i) {
    int val =
            0x00000001 * unsigned(code[i+3])
          + 0x00000100 * unsigned(code[i+2])
          + 0x00010000 * unsigned(code[i+1])
          + 0x01000000 * unsigned(code[i+0]);
    return val;
  }

  static void set32(byte[] code, int i, int val) {
    code[i+3] = signed(val >> 0);
    code[i+2] = signed(val >> 8);
    code[i+1] = signed(val >> 16);
    code[i+0] = signed(val >> 24);
  }

  public static void translate(ClassInfo classInfo, byte[] code) {
    // process all code bytes
    for(int i=0;i<code.length;i++) {
      int cmd = unsigned(code[i]);

      // code translations to reduce number of instructions
      if(cmd == OP_ASTORE)      cmd = OP_ISTORE;
      if(cmd == OP_ASTORE_0)    cmd = OP_ISTORE_0;
      if(cmd == OP_ASTORE_1)    cmd = OP_ISTORE_1;
      if(cmd == OP_ASTORE_2)    cmd = OP_ISTORE_2;
      if(cmd == OP_ASTORE_3)    cmd = OP_ISTORE_3;
      if(cmd == OP_ACONST_NULL) cmd = OP_ICONST_0;
      if(cmd == OP_ALOAD)       cmd = OP_ILOAD;
      if(cmd == OP_ALOAD_0)     cmd = OP_ILOAD_0;
      if(cmd == OP_ALOAD_1)     cmd = OP_ILOAD_1;
      if(cmd == OP_ALOAD_2)     cmd = OP_ILOAD_2;
      if(cmd == OP_ALOAD_3)     cmd = OP_ILOAD_3;

      if(cmd == OP_IFNULL)      cmd = OP_IFEQ;
      if(cmd == OP_IFNONNULL)   cmd = OP_IFNE;
      if(cmd == OP_ARETURN)     cmd = OP_IRETURN;

      // we don't need these conversions, since ints, bytes and
      // shorts are the same internal type
      if(cmd == OP_I2B)         cmd = OP_NOP;
      if(cmd == OP_I2C)         cmd = OP_NOP;
      if(cmd == OP_I2S)         cmd = OP_NOP;




      code[i] = signed(cmd);

      

      if(PARAMETER_BYTES[cmd] < 0) {
	System.out.println("Unsupported byte code: 0x" +
			   Integer.toHexString(cmd));
	System.exit(-1);
      }

      if(cmd == OP_LDC) { // load from constant pool (e.g. strings)
	int index = unsigned(code[i+1]);
	System.out.print("ldc #" + index);
	code[i+1] = signed(classInfo.getConstPool().
			   constantRelocate(index));
      }
 
      if((cmd == OP_GETFIELD)||(cmd == OP_PUTFIELD)) {
	int index = 256 * unsigned(code[i+1]) + unsigned(code[i+2]);
	System.out.print("get/putfield #"+ index);
	index = classInfo.getConstPool().constantRelocate(index);
	code[i+1] = signed(index>>8);
	code[i+2] = signed(index&0xff);
      }

      if((cmd == OP_GETSTATIC)||(cmd == OP_PUTSTATIC)) {
	int index = 256 * unsigned(code[i+1]) + unsigned(code[i+2]);
	System.out.print("get/putstatic #" + index);
	index = classInfo.getConstPool().constantRelocate(index);
	code[i+1] = signed(index>>8);
	code[i+2] = signed(index&0xff);

	// getstatic usually uses a reference to the object in question.
	// if the object is native, then this is just an id, that is to
	// be directly pushed onto the stack, thus we replace the getstatic
	// instruction with a push instruction
	if((cmd == OP_GETSTATIC)&&((index>>8) >= NativeMapper.lowestNativeId)) 
	  code[i] = signed(OP_SIPUSH);
      }
      
      if((cmd == OP_INVOKEVIRTUAL)||(cmd == OP_INVOKESPECIAL)||
	 (cmd == OP_INVOKESTATIC)) {
	int index = 256 * unsigned(code[i+1]) + unsigned(code[i+2]);
	System.out.print("invoke #" + index);
	index = classInfo.getConstPool().constantRelocate(index);
	code[i+1] = signed(index>>8);
	code[i+2] = signed(index&0xff);
     }
 
      if(cmd == OP_NEW) {
	int index = 256 * unsigned(code[i+1]) + unsigned(code[i+2]);
	System.out.print("new #" + index);
	index = classInfo.getConstPool().constantRelocate(index);
	code[i+1] = signed(index>>8);
	code[i+2] = signed(index&0xff);
      }

      if(cmd == OP_TABLESWITCH) {
        UsedFeatures.add(UsedFeatures.TABLESWITCH);
	//System.out.println("tableswitch");
        // opcode is followed by up to 3 padding bytes (replaced by nop's)
        // 4 byte low value follows
        // 4 byte hi value follows
        // (hi-lo+1) * 4 bytes build the table
        int delta=0;
        i++;
        while (i%4!=0) {
          code[i-1]=signed(OP_NOP);
          code[i]=signed(OP_LOOKUPSWITCH);
          i++;
          delta++;
        }
        set32(code, i, get32(code, i) - delta); // realloc default label
        i+=4;
        int lo = get32(code, i+0);
        int hi = get32(code, i+4);
        i+=8;
        System.out.println("tableswitch: number of cases ="+(hi-lo+1));
        for (int j=0; j<(hi-lo+1); j++){
          set32(code, i, get32(code, i) - delta); // realloc label
          i+=4;
        }
        i--;
      }

      if(cmd == OP_LOOKUPSWITCH) {
        UsedFeatures.add(UsedFeatures.LOOKUPSWITCH);
        //System.out.println("lookupswitch");
        // opcode is followed by up to 3 padding bytes
        // 4 byte default offset follows
        // 4 byte count follows
        // count * 2 * 4 bytes build the table
        int delta=0;
        i++;
        while (i%4!=0) {
          code[i-1]=signed(OP_NOP);
          code[i]=signed(OP_LOOKUPSWITCH);
          i++;
          delta++;
        }
        set32(code, i, get32(code, i) - delta); // realloc default label
        i+=4;
        int count = get32(code, i);
        i+=4;
        System.out.println("lookupswitch: number of cases = "+count);
        for (int j=0; j<count; j++){
          i+=4;  // skip value
          set32(code, i, get32(code, i) - delta); // realloc label
          i+=4;
        }
        i--;
      }
      
      


      // update used bits
      if(cmd == OP_TABLESWITCH)  UsedFeatures.add(UsedFeatures.TABLESWITCH);
      if(cmd == OP_LOOKUPSWITCH) UsedFeatures.add(UsedFeatures.LOOKUPSWITCH);
      if(cmd == OP_FCONST_0)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FCONST_1)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FCONST_2)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FLOAD)        UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FLOAD_0)      UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FLOAD_1)      UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FLOAD_2)      UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FLOAD_3)      UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_IALOAD)       UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_FALOAD)       UsedFeatures.add(UsedFeatures.ARRAY + UsedFeatures.FLOAT);
      if(cmd == OP_BALOAD)       UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_FSTORE)       UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FSTORE_0)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FSTORE_1)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FSTORE_2)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FSTORE_3)     UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_IASTORE)      UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_FASTORE)      UsedFeatures.add(UsedFeatures.ARRAY + UsedFeatures.FLOAT);
      if(cmd == OP_BASTORE)      UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_FADD)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FSUB)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FMUL)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FDIV)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FREM)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FNEG)         UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_I2F)          UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_F2I)          UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FCMPL)        UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FCMPG)        UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_FRETURN)      UsedFeatures.add(UsedFeatures.FLOAT);
      if(cmd == OP_NEWARRAY)     UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_ANEWARRAY)    UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_ARRAYLENGTH)  UsedFeatures.add(UsedFeatures.ARRAY);
      if(cmd == OP_DUP_X1)       UsedFeatures.add(UsedFeatures.EXTSTACK);
      if(cmd == OP_DUP_X2)       UsedFeatures.add(UsedFeatures.EXTSTACK);
      if(cmd == OP_DUP2_X1)      UsedFeatures.add(UsedFeatures.EXTSTACK);
      if(cmd == OP_DUP2_X2)      UsedFeatures.add(UsedFeatures.EXTSTACK);
      if(cmd == OP_SWAP)         UsedFeatures.add(UsedFeatures.EXTSTACK);
      i += PARAMETER_BYTES[cmd];
    }
  }
}
