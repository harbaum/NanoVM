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

#if defined(UNIX) || defined(__CC65__)
#include <stdio.h>
#include <stdlib.h>
#else
#include <avr/io.h>
#include "delay.h"
#endif

#include "types.h"
#include "config.h"
#include "debug.h"
#include "error.h"

#if defined(UNIX) || defined(__CC65__)
char *error_msg[] = {
  // unix message              avr error code
  "HEAP: illegal chunk size",        // A
  "HEAP: corrupted",                 // B
  "HEAP: out of memory",             // C
  "HEAP: chunk does not exist",      // D
  "HEAP: out of stack memory",       // E
  "HEAP: stack underrun",            // F
  "HEAP: out of identifiers",        // G
  "ARRAY: illegal type",             // H
  "NATIVE: unknown method",          // I
  "NATIVE: unknown class",           // J
  "NATIVE: illegal argument",        // K
  "NVMFILE: unsupported features or not a valid nvm file",   // L
  "NVMFILE: wrong nvm file version", // M
  "VM: illegal reference",           // N
  "VM: unsupported opcode",          // O
  "VM: division by zero",            // P
  "VM: stack corrupted",             // Q
};
#else
#include "uart.h"
#endif

void error(err_t code) {
#if defined(UNIX) || defined(__CC65__)
  printf("NanoVM error: %s\n", error_msg[code]);
  exit(-1);
#else

  uart_putc('E');
  uart_putc('R');
  uart_putc('R');
  uart_putc(':');
  uart_putc('A'+code);
  uart_putc('\n');

  for(;;) {
    // reset watchdog here if in use

#ifdef ASURO
    // yellow/red blinking status led
    PORTD |= _BV(2);
    PORTB |= _BV(0);
    delay(MILLISEC(250));
    PORTB &= ~_BV(0);
    delay(MILLISEC(250));
#endif
  }
#endif
}
