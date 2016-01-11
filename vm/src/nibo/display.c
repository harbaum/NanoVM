//
//  Nibo-NanoVM, a tiny java VM for the Nibo robot
//
//  Copyright (C) 2007 by Nils Springob <nils@nicai-systems.de>
//  Based on work by Benjamin Benz(c't-Bot) and Till Harbaum(NanoVM)
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

/*!
 *  @brief  Routinen zur Displaysteuerung
 *  @author Benjamin Benz (bbe@heise.de)
 *  @author modified by Nils Springob (nils@nicai-systems.de)
 *  @date   2007-01-26
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include <stdarg.h>
#include <stdio.h>

#include "nibo/display.h"
#include "nibo/delay.h"
#include "nibo/iodefs.h"


/*! Puffergroesse fuer eine Zeile in bytes */
#define DISPLAY_BUFFER_SIZE	(DISPLAY_LENGTH + 1)

#define DISPLAY_CLEAR 0x01      /*!< Kommando zum Löschen */
#define DISPLAY_CURSORHOME 0x02 /*!< Kommando für den Cursor */

/**! Output-Pins des Displays */
#define DISPLAY_OUT (_BV(IO_DISPLAY_RS_BIT)|_BV(IO_DISPLAY_RW_BIT)|_BV(IO_DISPLAY_EN_BIT))

/**! Input-Pins des Displays */
#define DISPLAY_IN (_BV(IO_DISPLAY_READY_BIT))

#define clear_display_output_bits() do {IO_DISPLAY_PORT &= ~DISPLAY_OUT;} while(0)

/*
 * Im Moment der Low-High-Flanke von ENABLE liest das Dislplay 
 * die Werte von RS und R/W ein. Ist zu diesem Zeitpunkt R/W=0, 
 * dann liest das Display mit der folgenden High-Low-Flanke von ENABLE 
 * den Datenbus ein (Schreibzyklus). 
 * War aber R/W=1, dann legt das Display ein Datenword auf den 
 * Datenbus (Lese-Zyklus), solange bis die High-Low-Flanke von ENABLE 
 * das Interface wieder deaktiviert.
 */

void display_init(){
  activate_output_bit(IO_DISPLAY_RS);
  activate_output_bit(IO_DISPLAY_RW);
  activate_output_bit(IO_DISPLAY_EN);
  activate_output_bit(IO_DISPLAY_CS1);
  activate_output_bit(IO_DISPLAY_CS2);
  activate_output_bit(IO_DISPLAY_RST);
  IO_DISPLAY_DDR = 0x00;

  clear_output_bit(IO_DISPLAY_RS);
  clear_output_bit(IO_DISPLAY_RW);
  clear_output_bit(IO_DISPLAY_EN);
  clear_output_bit(IO_DISPLAY_CS1);
  clear_output_bit(IO_DISPLAY_CS2);
  clear_output_bit(IO_DISPLAY_RST);
  delay_ms(1);
  set_output_bit(IO_DISPLAY_RST);
  display_wait(0);
  display_wait(1);
}


void display_write(uint8_t mode, uint8_t data){
  set_output_bitval(IO_DISPLAY_CS1, !(mode&0x01));
  set_output_bitval(IO_DISPLAY_CS2, mode&0x01);
  set_output_bitval(IO_DISPLAY_RS, mode&0x02);
  clear_output_bit(IO_DISPLAY_RW);
  IO_DISPLAY_DDR = 0xff;
  IO_DISPLAY_PORT = data;
  delay_us(1);
  set_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  clear_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  IO_DISPLAY_PORT = 0x00;
  IO_DISPLAY_DDR = 0x00;
}

uint8_t display_read(uint8_t mode){
  set_output_bitval(IO_DISPLAY_CS1, !(mode&0x01));
  set_output_bitval(IO_DISPLAY_CS2, mode&0x01);
  set_output_bitval(IO_DISPLAY_RS, mode&0x02);
  set_output_bit(IO_DISPLAY_RW);
  IO_DISPLAY_DDR = 0x00;
  delay_us(1);
  set_output_bit(IO_DISPLAY_EN);
  delay_us(4);
  uint8_t data = IO_DISPLAY_PIN;
  clear_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  return data;
}


// high level fuctions

void display_wait(uint8_t mode){
  mode &= 0x01;
  while (display_read(mode)&0x80){
  }
}

