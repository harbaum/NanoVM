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
//  vm.h
//

#ifndef VM_H
#define VM_H

#include "nvmtypes.h"
#include "heap.h"
#include "nvmfile.h"

// additional items to be allocated on heap during constructor call
#define VM_CLASS_CONST_ALLOC  1

void   vm_init(void);
void   vm_run(u16_t mref);

// expand types
void * vm_get_addr(nvm_ref_t ref);

#define nvm_int2stack(x) (~NVM_IMMEDIATE_MASK & (x))
nvm_int_t nvm_stack2int(nvm_stack_t val);

#define nvm_ref2stack(x) (x)
#define nvm_stack2ref(x) (x)

#ifdef NVM_USE_FLOAT
nvm_stack_t nvm_float2stack(nvm_float_t val);
nvm_float_t nvm_stack2float(nvm_stack_t val);
#endif


#endif // VM_H
