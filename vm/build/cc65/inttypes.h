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
// inttypes.h
//

#ifndef INTTYPES_H
#define INTTYPES_H

typedef signed   char     int8_t;
typedef unsigned char    uint8_t;
typedef signed   short   int16_t;
typedef unsigned short  uint16_t;
typedef signed   long    int32_t;
typedef unsigned long   uint32_t;
typedef unsigned char      _Bool;
typedef unsigned short uintptr_t;

#define PACKED

#endif // INTTYPES_H
