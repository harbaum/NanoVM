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
//  debug.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "types.h"
#include "config.h"
#include "debug.h"

#ifdef DEBUG

#if defined(UNIX) || defined(__CC65__)
bool_t debug_enabled = FALSE;

void debug_enable(bool_t enable) {
  debug_enabled = enable;
}

void debugf(const char *fmt, ...) {
  if(debug_enabled) {
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
  }
}
#endif // UNIX || __CC65__

bool_t debug_isprint(u08_t chr) {
  return(chr >= 32 && chr <= 127);
}

void debug_hexdump(const void *data, u16_t size) {
  u16_t i,n = 0, b2c;
  const u08_t *ptr = data;

  if(!size) return;

  DEBUGF("---- dump %d bytes\n", size);

  while(size>0) {
    DEBUGF(DBG16": ", n);

    b2c = (size>16)?16:size;

    for(i=0;i<b2c;i++)
      DEBUGF(DBG8" ", ptr[i]);

    DEBUGF("  ");

    for(i=0;i<(16-b2c);i++)
      DEBUGF("   ");

    for(i=0;i<b2c;i++)
      DEBUGF("%c", debug_isprint(ptr[i])?ptr[i]:'.');

    DEBUGF("\n");

    ptr  += b2c;
    size -= b2c;
    n    += b2c;
  }
  DEBUGF("\n");
}

#endif // DEBUG
