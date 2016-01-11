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

#ifndef ARRAY_H
#define ARRAY_H

#include "heap.h"

#define T_BOOLEAN 4
#define T_CHAR 	  5
#define T_FLOAT   6  // not allowed in mvm
#define T_DOUBLE  7  // not allowed in mvm
#define T_BYTE 	  8
#define T_SHORT   9
#define T_INT 	 10
#define T_LONG 	 11  // not allowed in mvm

heap_id_t   array_new(nvm_int_t length, u08_t type);
nvm_int_t   array_length(heap_id_t id);
void	    array_bastore(heap_id_t id, nvm_int_t index, nvm_byte_t value);
nvm_byte_t  array_baload(heap_id_t id, nvm_int_t index);
void        array_iastore(heap_id_t id, nvm_int_t index, nvm_int_t value);
nvm_int_t   array_iaload(heap_id_t id, nvm_int_t index);
#ifdef NVM_USE_FLOAT
void        array_fastore(heap_id_t id, nvm_int_t index, nvm_float_t value);
nvm_float_t array_faload(heap_id_t id, nvm_int_t index);
#endif

#endif // ARRAY_H
