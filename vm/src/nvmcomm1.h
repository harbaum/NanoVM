//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005-2006 by Till Harbaum <Till@Harbaum.org>,
//                             Oliver Schulz <whisp@users.sf.net>
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
// NVM-Comm is the NanoVM upload and communication protocol.
// NVM-Comm v1.0 is a simple protocol usable only for firmware upload.
// It's basically XMODEM with 16 bytes block size.
//


#ifndef NVMCOMM1_H
#define NVMCOMM1_H


#include "types.h"
#include "config.h"
#include "debug.h"


#if defined(NVM_USE_COMM) && !defined(NVMCOMM2)
#define NVMCOMM1
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef NVMCOMM1



#define LOADER_BLOCK_SIZE  16

typedef struct {
  u08_t soh;
  u08_t block, nblock;
  u08_t data[LOADER_BLOCK_SIZE];
  u08_t sum;
} PACKED loader_t;


u08_t uart_get_block(u08_t *data, u08_t len);



#endif // NVMCOMM1

#ifdef __cplusplus
}
#endif

#endif // NVMCOMM1_H
