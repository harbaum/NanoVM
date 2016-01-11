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
//  uart.c
//

#ifdef __CC65__
#include <conio.h>
#include <stdio.h>
#endif

#include "types.h"
#include "config.h"
#include "debug.h"

#include "uart.h"
#include "delay.h"

// unix uart emulation
#ifdef UNIX
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <sys/select.h>

struct termios old_t;

FILE *in = NULL, *out = NULL;

void uart_bye(void) {
#ifdef UART_PORT
  fclose(in);  // out is identical
#else
  // restore terminal settings
  tcsetattr( 0, TCSANOW, &old_t);
#endif
}

void uart_sigproc() {
  exit(-1);   // exit (and thus call uart_bye)
}

void uart_init(void) {
#ifdef UART_PORT
  out = in = fopen(UART_PORT, "w+b");
  if(!in) {
    printf("unable to open %s\n", UART_PORT);
    exit(-1);
  }

#else
  struct termios new_t;

  in = stdin;
  out = stdout;

  if(tcgetattr( 0, &old_t) == -1)
    perror("tcgetattr() failed");

  memcpy( &new_t, &old_t, sizeof(struct termios));

  tcflush( 0, TCIFLUSH);

  // no cr/lf translation
  new_t.c_iflag &= ~(ICRNL);

  // echo and kernel buffers off
  new_t.c_lflag &= ~(ECHO|ICANON);

  tcsetattr( 0, TCSANOW, &new_t);

  // libc buffers off
  setbuf(stdin, NULL);
#endif

  atexit(uart_bye);
  signal(SIGINT, uart_sigproc);
}

void uart_write_byte(u08_t byte) {
  fputc(byte, out);
  fflush(out);
}

u08_t uart_read_byte(void) {
  return fgetc(in);
}

// unix can't tell us how many bytes in the input buffer are,
// so just return one as long as there's data
u08_t uart_available(void) {
  fd_set fds;
  struct timeval tv = { 0, 100 };

  FD_ZERO(&fds);
  FD_SET(fileno(in), &fds);

  return (select(FD_SETSIZE, &fds, NULL, NULL, &tv) == 1)?1:0;
}

#endif  // UNIX

#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_BITRATE_CONFIG (u16_t)(((CLOCK/16l)/(UART_BITRATE))-1)
#define UART_BUFFER_SIZE  (1<<(UART_BUFFER_BITS))
#define UART_BUFFER_MASK  ((UART_BUFFER_SIZE)-1)

#if defined(ATMEGA168)
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define TXEN TXEN0
#define RXEN RXEN0
#define RXCIE RXCIE0
#define URSEL UMSEL00
#define UCSZ0 UCSZ00
#define UDR UDR0
#define UDRE UDRE0
#define SIG_UART_RECV SIG_USART_RECV
#endif

#if defined(NIBO)
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UDR UDR0

#define URSEL UBRR0H
#define SIG_UART_RECV SIG_UART0_RECV
/*
#define TXEN TXEN0
#define RXEN RXEN0
#define RXCIE RXCIE0
#define UCSZ0 UCSZ00
#define UDRE UDRE0
#define SIG_UART_RECV SIG_USART_RECV
*/
#endif


u08_t uart_rd, uart_wr;
u08_t uart_buf[UART_BUFFER_SIZE];

void uart_init(void) {
  uart_rd = uart_wr = 0;   // init buffers

  UBRRH = (u08_t)((UART_BITRATE_CONFIG>>8) & 0xf);
  UBRRL = (u08_t)((UART_BITRATE_CONFIG) & 0xff);

  UCSRA = 0;
  UCSRB =
    _BV(RXEN) | _BV(RXCIE) |          // enable receiver and irq
    _BV(TXEN);                        // enable transmitter

  UCSRC = _BV(URSEL) | (3 << UCSZ0);  // 8n1

  sei();
}

SIGNAL(SIG_UART_RECV) {
  /* irq driven input */
  uart_buf[uart_wr] = UDR;

  /* and increase write pointer */
  uart_wr = ((uart_wr+1) & UART_BUFFER_MASK);
}

u08_t uart_available(void) {
  return(UART_BUFFER_MASK & (uart_wr - uart_rd));
}

void uart_write_byte(u08_t byte) {
  /* Wait for empty transmit buffer */
  while(!(UCSRA & _BV(UDRE)));

  // asuro needs echo cancellation, since the ir receiver "sees"
  // the transmitter
#ifdef ASURO
  // disable receiver
  UCSRB &= ~(_BV(RXEN) | _BV(RXCIE));
#endif

  // start transmission
  UDR = byte;

#ifdef ASURO
  // Wait for empty transmit buffer
  while(!(UCSRA & _BV(UDRE)));
  delay(MILLISEC(5));

  // re-enable receiver
  UCSRB |= _BV(RXEN) | _BV(RXCIE);
#endif
}

u08_t uart_read_byte(void) {
  u08_t ret = uart_buf[uart_rd];

  /* and increase read pointer */
  uart_rd = ((uart_rd+1) & UART_BUFFER_MASK);

  return ret;
}

#endif // AVR

#ifdef __CC65__

u08_t uart_available(void) {
  return kbhit()?1:0;
}

// Use conio for available() support
u08_t uart_read_byte(void) {
  return cgetc();
}

// Use stdio for scrolling support
void uart_putc(u08_t byte) {
#ifdef __CBM__
  if((byte & 0x60) == 0x40) byte |= 0x80;
  if((byte & 0x60) == 0x60) byte &= 0xDF;
#endif
  putchar(byte);
}

#else // __CC65__

// translate nl to cr nl
void uart_putc(u08_t byte) {
  if(byte == '\n')
    uart_write_byte('\r');

  uart_write_byte(byte);
}

#endif // __CC65__
