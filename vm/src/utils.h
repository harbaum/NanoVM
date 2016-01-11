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
//  utils.h
//

#ifndef UTILS_H
#define UTILS_H

#ifdef NVM_USE_UTILS

static inline void utils_memcpy(void *dst, const void *src, u16_t len) {
  u08_t *dst8 = (u08_t*)dst;
  u08_t *src8 = (u08_t*)src;

  while(len--)
    *dst8++ = *src8++;
}

/* string len ram */
static inline u16_t utils_strlen(const char *str) {
  u16_t len=0;

  while(*str++) len++;
  return len;
}

#else // NVM_USE_UTILS

#define utils_memcpy memcpy
#define utils_strlen strlen

#endif // NVM_USE_UTILS

#endif // UTILS_H
