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

#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif


#include <inttypes.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef PACKED
#define PACKED __attribute__ ((packed))
#endif

typedef uint32_t  u32_t;
typedef int32_t   s32_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef uint8_t   u08_t;
typedef int8_t    s08_t;
typedef _Bool     bool_t;
typedef uintptr_t ptr_t;

typedef uint8_t   size8_t;  // A byte-sized size_t
typedef int8_t    ssize8_t; // A byte-sized ssize_t


#if !defined __cplusplus

#ifndef bool
typedef _Bool     bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef true
#define true (!false)
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef min
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef max
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#endif

#endif // !defined __cplusplus


#ifdef __cplusplus
}
#endif

#endif // TYPES_H
