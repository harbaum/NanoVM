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
//  error.h
//

#ifndef ERROR_H
#define ERROR_H

#define ERROR_HEAP_BASE                   0
#define ERROR_HEAP_ILLEGAL_CHUNK_SIZE     (ERROR_HEAP_BASE+0)
#define ERROR_HEAP_CORRUPTED              (ERROR_HEAP_BASE+1)
#define ERROR_HEAP_OUT_OF_MEMORY          (ERROR_HEAP_BASE+2)
#define ERROR_HEAP_CHUNK_DOES_NOT_EXIST   (ERROR_HEAP_BASE+3)
#define ERROR_HEAP_OUT_OF_STACK_MEMORY    (ERROR_HEAP_BASE+4)
#define ERROR_HEAP_STACK_UNDERRUN         (ERROR_HEAP_BASE+5)
#define ERROR_HEAP_OUT_OF_IDS             (ERROR_HEAP_BASE+6)

#define ERROR_ARRAY_BASE                  (ERROR_HEAP_BASE+7)
#define ERROR_ARRAY_ILLEGAL_TYPE          (ERROR_ARRAY_BASE+0)

#define ERROR_NATIVE_BASE                 (ERROR_ARRAY_BASE+1)
#define ERROR_NATIVE_UNKNOWN_METHOD       (ERROR_NATIVE_BASE+0)
#define ERROR_NATIVE_UNKNOWN_CLASS        (ERROR_NATIVE_BASE+1)
#define ERROR_NATIVE_ILLEGAL_ARGUMENT     (ERROR_NATIVE_BASE+2)

#define ERROR_NVMFILE_BASE                (ERROR_NATIVE_BASE+3)
#define ERROR_NVMFILE_MAGIC               (ERROR_NVMFILE_BASE+0)
#define ERROR_NVMFILE_VERSION             (ERROR_NVMFILE_BASE+1)

#define ERROR_VM_BASE                     (ERROR_NVMFILE_BASE+2)
#define ERROR_VM_ILLEGAL_REFERENCE        (ERROR_VM_BASE+0)
#define ERROR_VM_UNSUPPORTED_OPCODE       (ERROR_VM_BASE+1)
#define ERROR_VM_DIVISION_BY_ZERO         (ERROR_VM_BASE+2)
#define ERROR_VM_STACK_CORRUPTED          (ERROR_VM_BASE+3)

typedef u08_t err_t;

void error(err_t code);

#endif // ERROR_H
