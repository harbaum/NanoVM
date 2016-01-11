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
//  native_stdio.c
//

#include <string.h>

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef NVM_USE_STDIO

#include "utils.h"
#include "uart.h"
#include "vm.h"
#include "native.h"
#include "native_stdio.h"
#include "stack.h"
#include "nvmstring.h"

void native_itoa(char *str, nvm_int_t val) {
  nvm_int_t m;
#ifdef NVM_USE_16BIT_WORD
  nvm_int_t div = 10000;  // max num to be output with 15 bit integer 
#else  
  nvm_int_t div = 1000000000L;  // max num to be output 
#endif
  bool_t printed = FALSE;
  
  if(val < 0) {
    *str++ = '-';
    val = -val;
  }
  
  while(div > 0) {
    m = val / div;
    if(m) printed = TRUE;
    
    if((printed)||(div == 1)) 
      *str++ = '0' + m;
    
    val = val % div;
    div /= 10;
  }
  
  *str++ = 0;
}

#ifdef NVM_USE_FLOAT
void native_ftoa(char *str, nvm_float_t val) {
  if (val<0)
  {
    *str++='-';
    val=-val;
  }

  nvm_int_t i = val;
  native_itoa(str, i);
  while (*str) str++; // to string end
  char * sep = str;
  val+=1-i;
  i=val*1000000;
  native_itoa(str, i);
  sep[0] ='.';
  while (*str) str++; // to string end
  while (*(--str)=='0') *str=0;
  if (*str=='.') *str=0;
}
#endif

// send a string to the console and append return if ret is true
static void native_print(char *str, bool_t ret) {
#ifdef NVM_USE_EEPROM
  u08_t chr;
  // check if source string is within internal nvm file, otherwise 
  // it's directly being read from ram
  if(NVMFILE_ISSET(str)) {
    while((chr = nvmfile_read08(str++)))
      uart_putc(chr);
  } else
#endif
    while(*str)
      uart_putc(*str++);

  if(ret)
    uart_putc('\n');
}

// invoke a native method within class java/io/PrintStream
void native_java_io_printstream_invoke(u08_t mref) {
  char tmp[8];

  if(mref == NATIVE_METHOD_PRINTLN_STR) {
    native_print(stack_pop_addr(), TRUE);
  } else if(mref == NATIVE_METHOD_PRINTLN_INT) {
    native_itoa((char*)tmp, stack_pop_int());
    native_print(tmp, TRUE);
  } else if(mref == NATIVE_METHOD_PRINT_STR) {
    native_print(stack_pop_addr(), FALSE);
  } else if(mref == NATIVE_METHOD_PRINT_INT) {
    native_itoa((char*)tmp, stack_pop_int());
    native_print(tmp, FALSE);
  } else if(mref == NATIVE_METHOD_PRINTLN_CHAR) {
    uart_putc(stack_pop_int());
    uart_putc('\n');
  } else if(mref == NATIVE_METHOD_PRINT_CHAR) {
    uart_putc(stack_pop_int());
#ifdef NVM_USE_EXT_STDIO
  } else if(mref == NATIVE_METHOD_FORMAT) {
    native_print(stack_pop_addr(), FALSE);
    stack_pop_int(); // TODO
    stack_push(stack_peek(0)); // duplicate this ref
#endif    
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);

  // popping the reference at the end only works as long
  // as none of the native methods places anything on the stack
  
  stack_pop(); // pop output stream reference
}

// invoke a native method within class java/io/InputStream
void native_java_io_inputstream_invoke(u08_t mref) {

  // popping the reference at the beginning only works as long
  // as all of the native methods place something on the stack

  stack_pop(); // pop input stream reference

  if(mref == NATIVE_METHOD_INPUTSTREAM_AVAILABLE) {
    stack_push(uart_available());
  } else if(mref == NATIVE_METHOD_INPUTSTREAM_READ) {
    stack_push(uart_read_byte());
  } else 
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}    

// invoke a native method within class java/lang/StringBuffer
void native_java_lang_stringbuffer_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_INIT) {
    // make this an empty string
    *(char*)stack_pop_addr() = 0;
  } else if(mref == NATIVE_METHOD_INIT_STR) {
    char *src, *dst;
    u16_t len;

    src = stack_peek_addr(0);
    // check source of string
    len = native_strlen(src);

    // resize existing object
    heap_realloc(stack_peek(1) & ~NVM_TYPE_MASK, len + 1);

    // and copy string to new object
    src = stack_peek_addr(0);
    dst = heap_get_addr(stack_peek(1) & ~NVM_TYPE_MASK);
    native_strcpy(dst, src);

    // get rid of source references still on the stack
    stack_pop(); stack_pop(); 

  } else if((mref == NATIVE_METHOD_APPEND_STR)||
	    (mref == NATIVE_METHOD_APPEND_INT)||
	    (mref == NATIVE_METHOD_APPEND_CHR)||
            (mref == NATIVE_METHOD_APPEND_FLOAT)) {
    char *src0, *src1, *dst;
#ifdef NVM_USE_FLOAT
    char tmp[15];
#else
# ifdef NVM_USE_32BIT_WORD
    char tmp[10];
# else
    char tmp[5];
# endif
#endif
    u16_t len;
    heap_id_t id;
    
    if(mref == NATIVE_METHOD_APPEND_STR) {
      // appending a string is simple
      src1 = stack_peek_addr(0);
      // check source of string
      len = native_strlen(src1);
    } else {
      if(mref == NATIVE_METHOD_APPEND_INT) {
        // integer has to be converted
        native_itoa(tmp, stack_peek_int(0));
#ifdef NVM_USE_FLOAT
      } else if(mref == NATIVE_METHOD_APPEND_FLOAT) {
        // integer has to be converted
        native_ftoa(tmp, stack_peek_float(0));
#endif
      } else {
	// character is directly appended
	tmp[0] = stack_peek(0);
	tmp[1] = 0;
      }

      src1 = tmp;
      len = utils_strlen((char*)src1);
    }

    // this is always a string
    src0 = stack_peek_addr(1);
    len += native_strlen(src0);
    
    // create a new object 
    id = heap_alloc(FALSE, len + 1);

    // alloc may have had an impact on heap, so get address again
    src0 = stack_peek_addr(1);
    if(mref == NATIVE_METHOD_APPEND_STR) 
      src1 = stack_peek_addr(0);

    // handle nvmfile memory and ram
    dst = heap_get_addr(id);
    native_strcpy(dst, src0);
    native_strcat(dst, src1);

    // get rid of source references still on the stack
    stack_pop(); stack_pop();  // 
    // place new reference on the stack
    stack_push(NVM_TYPE_HEAP | id);

  } else if(mref == NATIVE_METHOD_TOSTRING) {
    // toString does nothing, since the object already is
    // a valid string
  } else {
    DEBUGF("unknown method in java/lang/StringBuffer\n");
    error(ERROR_NATIVE_UNKNOWN_METHOD);
  }
}
  
#endif // NVM_USE_STDIO
