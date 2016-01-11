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
//  nvmstring.h
//

#ifndef NVMSTRING_H
#define NVMSTRING_H

#ifdef NVM_USE_EEPROM

void native_strcpy(char *dst, const char* src);
void native_strncpy(char *dst, const char* src, int n);
u16_t native_strlen(const char *str);
void native_strcat(char *dst, const char *src);
void native_strncat(char *dst, const char *src, int n);
char native_getchar(const char* src);

#else // NVM_USE_EEPROM

#define native_strcpy       strcpy
#define native_strncpy      strncpy
#define native_strlen       strlen
#define native_strcat       strcat
#define native_strncat      strncat
#define native_getchar(src) (*(src))

#endif // NVM_USE_EEPROM

#endif // NVMSTRING_H
