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


#include "nvmcomm1.h"

#include "uart.h"
#include "delay.h"

#ifdef ASURO
#include <avr/io.h>
#endif


#ifdef NVMCOMM1


u08_t uart_get_block(u08_t *data, u08_t len) {
  u08_t timeout, cnt = 0;

  while(len) {
    timeout = 10;  // 10 * 20ms
    while(!uart_available()) {
      // reset watchdog here if enabled
      delay(MILLISEC(20));
      if(timeout-- == 0) return cnt;
    }

    *data++ = uart_read_byte();
    len--;
    cnt++;
  }
  return cnt;
}


#endif // NVMCOMM1
