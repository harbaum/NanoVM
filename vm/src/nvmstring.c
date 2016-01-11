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
//  string.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef NVM_USE_EEPROM

#ifdef NVM_USE_STDIO

#include "vm.h"
#include "stack.h"
#include "nvmstring.h"

#ifdef NVM_USE_FORMATTER

char native_getchar(const char* src) {
  // check if string resides in nvm file memory (e.g. eeprom)
  if(NVMFILE_ISSET(src))
    return nvmfile_read08(src);
  else
    return *src;
}

/* string copy to ram */
void native_strncpy(char *dst, const char* src, int n) {
  // check if string resides in nvm file memory (e.g. eeprom)
  if(NVMFILE_ISSET(src))
    while(n--&&(*dst++ = nvmfile_read08(src++)));
  else
    while(n--&&(*dst++ = *src++));
}

// append a string to another one
void native_strncat(char *dst, const char *src, int n) {
  while(n--&&(*dst)) dst++;         // skip string
  native_strncpy(dst, src, n);   // attach it
}

#endif // NVM_USE_FORMATTER

/* string copy to ram */
void native_strcpy(char *dst, const char* src) {
  // check if string resides in nvm file memory (e.g. eeprom)
  if(NVMFILE_ISSET(src))
    while((*dst++ = nvmfile_read08(src++)));
  else
    while((*dst++ = *src++));
}

/* determine string length */
u16_t native_strlen(const char *str) {
  u16_t len=0;

  // check if string resides in nvm file memory (e.g. eeprom)
  if(NVMFILE_ISSET(str))
    while(nvmfile_read08(str++)) len++;
  else
    while(*str++) len++;
  
  return len;
}

// append a string to another one
void native_strcat(char *dst, const char *src) {
  while(*dst) dst++;         // skip string
  native_strcpy(dst, src);   // attach it
}

#endif // NVM_USE_STDIO

#endif // NVM_USE_EEPROM
