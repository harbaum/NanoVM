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
//  This file contains the heap. It can be requested to
//  create/delete objects on the heap and does some
//  simple garbage collection.
//
//  The heap is being used top-to-bottom allowing the
//  virtual machines stack to grow inside the heap from bottom to
//  top
//

#include <string.h>

#include "types.h"
#include "config.h"
#include "debug.h"
#include "error.h"

#include "utils.h"
#include "heap.h"
#include "stack.h"
#include "vm.h"

u08_t heap[HEAPSIZE];
u16_t heap_base = 0;

typedef struct {
  heap_id_t id;
  u16_t len;  // actually 15 bits for the len and 1 bit for the fieldref flag
} PACKED heap_t;

#define HEAP_ID_FREE       0
#define HEAP_LEN_MASK      0x7FFF
#define HEAP_FIELDREF_MASK 0x8000

#ifdef NVM_USE_HEAP_IDMAP
// A heap id map must not be larger than 256 elements meaning a limit of 2048
// heap elements. Each heap element consists of the header (heap_t) and the
// actual data which makes up at least 1 byte. Limiting the heap size to 10kB
// makes sure that are always heap ids available.
#if HEAPSIZE > 10240
#error The maximum heap size is 10kB when using a heap id map.
#endif
u08_t heap_idmap[(HEAPSIZE/(sizeof(heap_t)+1))/8];
const u08_t heap_idmap_mask[8] = {0x01, 0x02, 0x04, 0x08,
                                  0x10, 0x20, 0x40, 0x80};
#endif

// return the real heap base (where memory can be "stolen"
// from
u08_t *heap_get_base(void) {
  return heap;
}

#ifdef NVM_USE_MEMCPY_UP
// a version of memcpy that can only copy overlapping chunks
// if the target address is higher
void heap_memcpy_up(u08_t *dst, u08_t *src, u16_t len) {
  dst += len;  src += len;
  while(len--) *--dst = *--src;
}
#endif

#ifdef DEBUG_JVM
// make some sanity checks on the heap in order to detect 
// heap corruption as early as possible
void heap_check(void) {
  u16_t current = heap_base;
  heap_t *h = (heap_t*)&heap[current];
  u16_t len;

  if(h->id != HEAP_ID_FREE) {
    DEBUGF("heap_check(): start element not free element\n");
    error(ERROR_HEAP_CORRUPTED);
  }  
  
  // (no HEAP_LEN_MASK required for free chunk)
  current += h->len + sizeof(heap_t);

  while(current < sizeof(heap)) {
    h = (heap_t*)&heap[current];
    len = h->len & HEAP_LEN_MASK;
    if(len > sizeof(heap)) {
      DEBUGF("heap_check(): single chunk too big\n");
      heap_show();
      error(ERROR_HEAP_ILLEGAL_CHUNK_SIZE);
    }
    
    if(len + sizeof(heap_t) > sizeof(heap) - current) {
      DEBUGF("heap_check(): total size error\n");
      heap_show();
      error(ERROR_HEAP_CORRUPTED);
    }

    current += len + sizeof(heap_t);
  }

  if(current != sizeof(heap)) {
    DEBUGF("heap_check(): heap sum mismatch\n");
    heap_show();
    error(ERROR_HEAP_CORRUPTED);
  }
}
#endif

#ifdef UNIX
void heap_show(void) {
  u16_t current = heap_base;

  DEBUGF("Heap:\n");
  while(current < sizeof(heap)) {
    heap_t *h = (heap_t*)&heap[current];
    u16_t len = h->len & HEAP_LEN_MASK;
    if(h->id == HEAP_ID_FREE) {
      DEBUGF("- %d free bytes\n", len);
    } else {
      DEBUGF("- chunk id %x with %d bytes:\n", h->id, len);

      if(len > sizeof(heap))
	error(ERROR_HEAP_ILLEGAL_CHUNK_SIZE);

      DEBUG_HEXDUMP(h+1, len);
    }

    if(len + sizeof(heap_t) > sizeof(heap) - current) {
      DEBUGF("heap_show(): total size error\n");
      error(ERROR_HEAP_CORRUPTED);
    }

    current += len + sizeof(heap_t);
  }

  DEBUGF("- %d bytes stolen\n", heap_base);
}
#endif

// search for chunk with id in heap and return chunk header
// address
heap_t *heap_search(heap_id_t id) {
  u16_t current = heap_base;

  while(current < sizeof(heap)) {
    heap_t *h = (heap_t*)&heap[current];
    if(h->id == id) return h;
    current += (h->len & HEAP_LEN_MASK) + sizeof(heap_t);
  }
  return NULL;
}

#ifdef NVM_USE_HEAP_IDMAP

void heap_init_ids(void) {
  memset(heap_idmap, 0, sizeof(heap_idmap));
  heap_idmap[0] = 0x01;  // mark HEAP_ID_FREE
}

void heap_mark_id(heap_id_t id) {
  DEBUGF("  heap_mark_id(id=0x%04x)\n", id);
  heap_idmap[id/8] |= heap_idmap_mask[id%8];
}

u08_t heap_id_marked(heap_id_t id) {
  return heap_idmap[id/8] & heap_idmap_mask[id%8];
}

heap_id_t heap_new_id(void) {
  u08_t byte,bit;

  for(byte=0;;byte++) {
    if(heap_idmap[byte] != 0xFF)
      for(bit=0;;bit++)
	if(!(heap_idmap[byte] & heap_idmap_mask[bit])) {
	  heap_idmap[byte] |= heap_idmap_mask[bit];
	  return byte*8+bit;
	}

    // check failure here before incrementing
    // to allow for id maps with 256 elements
    if(byte == sizeof(heap_idmap)-1)
      return 0;
  }
}

// in some cases, references to heap objects may be inside
// other heap objects. this currently happens only when
// a class is instanciated and this class contains fields.
// the heap element created by the constructor is marked with
// the fieldref bit and it is searched for references during
// garbage collections
void heap_mark_child_ids(void) {
  bool_t again;

  do {
    u16_t current = heap_base;
    again = FALSE;

    DEBUGF("heap_mark_child_ids(): starting heap walk\n");
    while(current < sizeof(heap)) {
      heap_t *h = (heap_t*)&heap[current];

      // check for elements with the fieldref flag
      if(h->len & HEAP_FIELDREF_MASK && heap_id_marked(h->id)) {
	u08_t fields = (u08_t)((h->len & HEAP_LEN_MASK) / sizeof(nvm_ref_t));
	u08_t i;

	// check all fields in the heap element
	DEBUGF("- checking id 0x%04x\n", h->id);
	for(i=0;i<fields;i++) {
	  nvm_ref_t ref = ((nvm_ref_t*)(h+1))[i];

	  // mark heap element only if field actually references
	  // a heap element and that wasn't already marked before
	  if((ref & NVM_TYPE_MASK) == NVM_TYPE_HEAP &&
	     !heap_id_marked(ref & ~NVM_TYPE_MASK)) {
	    heap_mark_id(ref & ~NVM_TYPE_MASK);

	    // we could check here if any of the newly marked heap elements
	    // has the fieldref flag set but doing would require walking the
	    // heap for every single one (!) so it's cheaper to just do the
	    // walk here again as soon as we newly marked any heap elements
	    again = TRUE;
	  }
	}
      }

      current += (h->len & HEAP_LEN_MASK) + sizeof(heap_t);
    }
  } while(again);  
}

#else // NVM_USE_HEAP_IDMAP

heap_id_t heap_new_id(void) {
  heap_id_t id;

  for(id=1;id;id++) 
    if(heap_search(id) == NULL) 
      return id;

  return 0;
}

// in some cases, references to heap objects may be inside
// other heap objects. this currently happens only when
// a class is instanciated and this class contains fields.
// the heap element created by the constructor is marked with
// the fieldref bit and it is searched for references during
// garbage collections
bool_t heap_fieldref(heap_id_t id) {
  nvm_ref_t id16 = id | NVM_TYPE_HEAP;
  u16_t current = heap_base;

  // walk through the entire heap
  while(current < sizeof(heap)) {
    heap_t *h = (heap_t*)&heap[current];

    // check for entries with the fieldref flag
    if(h->len & HEAP_FIELDREF_MASK) {
      u08_t entries = (u08_t)((h->len & HEAP_LEN_MASK) / sizeof(nvm_ref_t));
      u08_t i;

      // check all entries in the heap element for
      // the reference we are searching for
      for(i=0;i<entries;i++) {
	if(((nvm_ref_t*)(h+1))[i] == id16)
	  return TRUE;
      }
    }

    current += (h->len & HEAP_LEN_MASK) + sizeof(heap_t);
  }
  
  return FALSE;
}

#endif // NVM_USE_HEAP_IDMAP

bool_t heap_alloc_internal(heap_id_t id, bool_t fieldref, u16_t size) {
  u16_t req = size + sizeof(heap_t);  // total mem required

  // search for free block
  heap_t *h = (heap_t*)&heap[heap_base];

  // (no HEAP_LEN_MASK required for free chunk)
  if(h->len >= req) {
    // reduce the size of the free chunk
    // (no HEAP_LEN_MASK required for free chunk)
    h->len -= req;

    // and create the new chunk behind this one
    // (no HEAP_LEN_MASK required for free chunk)
    h = (heap_t*)&heap[heap_base + sizeof(heap_t) + h->len];
    h->id = id;
    h->len = fieldref ? size | HEAP_FIELDREF_MASK : size & HEAP_LEN_MASK;
#ifdef NVM_INITIALIZE_ALLOCATED
    // fill memory with zero
    u08_t *ptr = (void*)(h+1);
    while(size--)
      *ptr++ = 0;
#endif
    return TRUE;
  }

  DEBUGF("heap_alloc_internal(%d): out of memory\n", size);
  return FALSE;
}

heap_id_t heap_alloc(bool_t fieldref, u16_t size) {
  heap_id_t id = heap_new_id();

  DEBUGF("heap_alloc(size=%d) -> id=0x%04x\n", size, id);
  if(!id) error(ERROR_HEAP_OUT_OF_IDS);

  if(!heap_alloc_internal(id, fieldref, size)) {
    heap_garbage_collect();

    // we need to reallocate heap id, gc. threw away the old one...
    id = heap_new_id();
    if(!id) error(ERROR_HEAP_OUT_OF_IDS);

    if(!heap_alloc_internal(id, fieldref, size))
      error(ERROR_HEAP_OUT_OF_MEMORY);
    DEBUGF("heap_alloc(size=%d) -> id=0x%04x successfull after gc\n",
           size, id);
  }

  return id;
}

void heap_realloc(heap_id_t id, u16_t size) {
  heap_t *h, *h_new;

  DEBUGF("heap_realloc(id=0x%04x, size=%d)\n", id, size);

  // check free mem and call garbage collection if required
  h = (heap_t*)&heap[heap_base];
  // (no HEAP_LEN_MASK required for free chunk)
  if(h->len < size + sizeof(heap_t))
    heap_garbage_collect();

  // get info on old chunk
  h = heap_search(id);

  // allocate space for bigger one
  if(!heap_alloc_internal(id, h->len & HEAP_FIELDREF_MASK ? TRUE : FALSE, size))
    error(ERROR_HEAP_OUT_OF_MEMORY);

  h_new = heap_search(id);

  utils_memcpy(h_new+1, h+1, h->len & HEAP_LEN_MASK);

  // this chunk is not immediately available for new allocation
  // but it will be removed by the garbage collection next time 
  h->id = HEAP_ID_FREE;
}

u16_t heap_get_len(heap_id_t id) {
  heap_t *h = heap_search(id);
  if(!h) error(ERROR_HEAP_CHUNK_DOES_NOT_EXIST);
  return h->len & HEAP_LEN_MASK;
}

void *heap_get_addr(heap_id_t id) {
  heap_t *h = heap_search(id);
  if(!h) error(ERROR_HEAP_CHUNK_DOES_NOT_EXIST);
  return h+1;
}

void heap_init(void) {
  heap_t *h;

  DEBUGF("heap_init()\n");

  // just one big free block
  h = (heap_t*)&heap[0];
  h->id = HEAP_ID_FREE;
  // (no HEAP_LEN_MASK required for free chunk)
  h->len = sizeof(heap) - sizeof(heap_t);

#ifdef NVM_USE_HEAP_IDMAP
  heap_init_ids();
#endif
}

// walk through the heap, check for every object
// if it's still being used and remove it if not
void heap_garbage_collect(void) {
  u16_t current = heap_base;
  heap_t *h;
  // (no HEAP_LEN_MASK required for free chunk)
  DEBUGF("heap_garbage_collect() free space before: %d\n", ((heap_t*)&heap[heap_base])->len);

#ifdef NVM_USE_HEAP_IDMAP
  heap_init_ids();
  stack_mark_heap_root_ids();
  heap_mark_child_ids();
#endif

  // set current to stack-top
  // walk through the entire heap
  while(current < sizeof(heap)) {
    u16_t len;
    h = (heap_t*)&heap[current];
    len = (h->len & HEAP_LEN_MASK) + sizeof(heap_t);

    // found an entry
    if(h->id != HEAP_ID_FREE) {
      // check if it's still used
#ifdef NVM_USE_HEAP_IDMAP
      if(!heap_id_marked(h->id)) {
#else
      if((!stack_heap_id_in_use(h->id))&&(!heap_fieldref(h->id))) {
#endif
	// it is not used, remove it
	DEBUGF("HEAP: removing unused object with id 0x%04x (len %d)\n",
	       h->id, len);
      
	// move everything before to the top
#ifdef NVM_USE_MEMCPY_UP
	heap_memcpy_up(heap+heap_base+len, heap+heap_base, current-heap_base);
#else
	memmove(heap+heap_base+len, heap+heap_base, current-heap_base);
#endif

	// add freed mem to free chunk
	h = (heap_t*)&heap[heap_base];
	// (no HEAP_LEN_MASK required for free chunk)
	h->len += len;
      }
    }
    current += len;
  }

  if(current != sizeof(heap)) {
    DEBUGF("heap_garbage_collect(): total size error\n");
    error(ERROR_HEAP_CORRUPTED);
  }

  // (no HEAP_LEN_MASK required for free chunk)
  DEBUGF("heap_garbage_collect() free space after: %d\n", ((heap_t*)&heap[heap_base])->len);
}

// "steal" some bytes from the bottom of the heap (where
// the free chunk is)
void heap_steal(u16_t bytes) {
  heap_t *h = (heap_t*)&heap[heap_base];
  u16_t len;

  DEBUGF("HEAP: request to steal %d bytes\n", bytes);

  if(h->id != HEAP_ID_FREE) {
    DEBUGF("heap_steal(%d): start element not free element\n", bytes);
    error(ERROR_HEAP_CORRUPTED);
  }

  // try to make space if necessary
  // (no HEAP_LEN_MASK required for free chunk)
  len = h->len;
  if(len < bytes) 
    heap_garbage_collect();

  // (no HEAP_LEN_MASK required for free chunk)
  len = h->len;
  if(len < bytes) 
    error(ERROR_HEAP_OUT_OF_STACK_MEMORY);

   // finally steal ...
  heap_base += bytes;
  h = (heap_t*)&heap[heap_base];
  h->id = HEAP_ID_FREE;
  // (no HEAP_LEN_MASK required for free chunk)
  h->len = len - bytes;
}

// someone wants us to give some bytes back :-)
void heap_unsteal(u16_t bytes) {
  heap_t *h = (heap_t*)&heap[heap_base];
  u16_t len;

  if(h->id != HEAP_ID_FREE) {
    DEBUGF("heap_unsteal(%d): start element not free element\n", bytes);
    error(ERROR_HEAP_CORRUPTED);
  }

  DEBUGF("HEAP: request to unsteal %d bytes\n", bytes);

  if(heap_base < bytes) {
    DEBUGF("stack underrun by %d bytes\n", bytes - heap_base);
    error(ERROR_HEAP_STACK_UNDERRUN);
  }

  // finally unsteal ...
  // (no HEAP_LEN_MASK required for free chunk)
  len = h->len;
  heap_base -= bytes;
  h = (heap_t*)&heap[heap_base];
  h->id = HEAP_ID_FREE;
  // (no HEAP_LEN_MASK required for free chunk)
  h->len = len + bytes;
}
