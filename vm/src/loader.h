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
//  loader.h
//

#ifndef LOADER_H
#define LOADER_H

#define ASCII_SOH  0x01 
#define ASCII_STX  0x02 
#define ASCII_EOT  0x04
#define ASCII_ACK  0x06 
#define ASCII_DLE  0x10
#define ASCII_XON  0x11 
#define ASCII_XOFF 0x13
#define ASCII_NAK  0x15 
#define ASCII_SYN  0x16
#define ASCII_CAN  0x18 
#define ASCII_EOF  0x1A 

void loader_receive(void);

#endif // LOADER_H
