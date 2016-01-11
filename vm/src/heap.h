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

#ifndef HEAP_H
#define HEAP_H

// Each heap element consists of a header and the actual data. This header
// consists of the heap id and a 2 byte combined length/flag value. With a
// 1 byte heap id and and the border case of 1 byte data each heap element
// occupies 4 bytes so with a heap size <= 1024 bytes there are never more
// than 256 heap elements possible meaning a 1 byte heap id is sufficient.
#if HEAPSIZE <= 1024
typedef u08_t heap_id_t;
#else
typedef u16_t heap_id_t;
#endif 

void      heap_init(void);
u08_t     *heap_get_base(void);
heap_id_t heap_alloc(bool_t fieldref, u16_t size);
void      heap_realloc(heap_id_t id, u16_t size);
u16_t     heap_get_len(heap_id_t id);
void      *heap_get_addr(heap_id_t id);
//hey, this is java!!!  void      heap_free(heap_id_t id);
void      heap_garbage_collect(void);
void      heap_steal(u16_t bytes);
void      heap_unsteal(u16_t bytes);

#ifdef DEBUG_JVM
void      heap_check(void);
#define HEAP_CHECK()  heap_check()
#else
#define HEAP_CHECK() 
#endif

#ifdef UNIX
void      heap_show(void);
#endif

#ifdef NVM_USE_HEAP_IDMAP
void      heap_mark_id(heap_id_t id);
#endif

#endif // HEAP_H
