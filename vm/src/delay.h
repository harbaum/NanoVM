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
//  delay.h
//
//  delay and clock rate related routines
//

#ifndef DELAY_H
#define DELAY_H

#include "types.h"
#include "config.h"

#ifdef UNIX
#include <unistd.h>

#define MICROSEC(a) (a)
#define MILLISEC(a) ((a)*1000)
#define SEC(a)      ((a)*1000000)

#define delay(a)  usleep(a)
#else

#ifdef AVR
 
#define MICROSEC(a) ((a)*CLOCK/7000000)
#define MILLISEC(a) ((a)*CLOCK/7000)
#define SEC(a)      ((a)*CLOCK/7)

//#define nop() ({__asm__ __volatile__ ("nop\n\t":);})

/* nop/1, subi/1, sbc/1, sbc/1, sbc/1, brcc/2 -> 7 clocks */
static inline void delay(u32_t delay_cnt) {
  while(delay_cnt--) {__asm__ __volatile__ ("nop\n\t":);}
}

#endif // nAVR
#endif // nUNIX

#endif // DELAY_H
