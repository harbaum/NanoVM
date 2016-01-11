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
//  nvmfile.h
//

#ifndef NVMFILE_H
#define NVMFILE_H

#include "types.h"
#include "vm.h"

typedef struct {
  u08_t super;
  u08_t fields;
} PACKED nvm_class_hdr_t;

typedef struct {
  u16_t code_index;
  u16_t id;           // class and method id
  u08_t flags;
  u08_t args;
  u08_t max_locals;
  u08_t max_stack;
} PACKED nvm_method_hdr_t;

typedef struct {
  u32_t magic_feature;    // old 32 bit magic is replaced by 8 bit magic and 24 feauture bits
  u08_t version;
  u08_t methods;          // number of methods in this file
  u16_t main;             // index of main method
  u16_t constant_offset;
  u16_t string_offset;
  u16_t method_offset;
  u08_t static_fields;
  nvm_class_hdr_t class_hdr[];
} PACKED nvm_header_t;

// marker that indicates, that a method is a classes init method
#define FLAG_CLINIT 1

extern u08_t nvmfile_constant_count;

void   nvmfile_store(u16_t index, u08_t *buffer, u16_t size);

bool_t nvmfile_init(void);
void   nvmfile_call_main(void);
void   *nvmfile_get_addr(u16_t ref);
u08_t  nvmfile_get_class_fields(u08_t index);
u08_t  nvmfile_get_static_fields(void);
u32_t  nvmfile_get_constant(u08_t index);
void   *nvmfile_get_base(void);
u08_t  nvmfile_get_method_by_class_and_id(u08_t class, u08_t id);
nvm_method_hdr_t *nvmfile_get_method_hdr(u16_t index);

#ifdef NVM_USE_EEPROM

void   nvmfile_read(void *dst, const void *src, u16_t len);
u08_t  nvmfile_read08(const void *addr);
u16_t  nvmfile_read16(const void *addr);
u32_t  nvmfile_read32(const void *addr);
void   nvmfile_write08(void *addr, u08_t data);

#define NVMFILE_SET(a)     (void*)(((ptr_t)a) | NVMFILE_FLAG)
#define NVMFILE_ISSET(a)   (((ptr_t)a) & NVMFILE_FLAG)
#define NVMFILE_ADDR(a)    (void*)(((ptr_t)a) & ~NVMFILE_FLAG)

#else // NVM_USE_EEPROM

#define nvmfile_read                memcpy
#define nvmfile_read08(addr)        (*(u08_t*)(addr))
#define nvmfile_read16(addr)        (*(u16_t*)(addr))
#define nvmfile_read32(addr)        (*(u32_t*)(addr))
#define nvmfile_write08(addr, data) (*(u08_t*)(addr) = (data))

#define NVMFILE_SET(addr)           (addr)

#endif // NVM_USE_EEPROM

#ifndef NVM_USE_FLASH_PROGRAM
#define nvmfile_write_initialize() do {} while(0)
#define nvmfile_write_finalize() do {} while(0)
#else
void nvmfile_write_initialize(void);
void nvmfile_write_finalize(void);
#endif

#ifdef NVM_USE_DISK_FILE
void nvmfile_load(const char *filename, bool_t quiet);
#endif

#endif // NVMFILE_H
