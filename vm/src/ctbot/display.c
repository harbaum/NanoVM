//
//  c't-Bot-NanoVM, a tiny java VM for the c't-Bot
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

#include "ct-Bot.h"

#ifdef MCU
#ifdef DISPLAY_AVAILABLE

#include <avr/io.h>
#ifdef NEW_AVR_LIB
# include <util/delay.h>
#else
# include <avr/delay.h>
#endif
#include <stdarg.h>
#include <stdio.h>

#include "ctbot/display.h"
#include "ctbot/shift.h"
#include "ctbot/delay.h"
#include "ctbot/iodefs.h"


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

/*! 
 * Übertrage Kommando an das Display
 * @param cmd Kommando
 */
void display_cmd(uint8_t cmd){		//ein Kommando cmd an das Display senden
    shift_data_out(cmd, SHIFT_LATCH, 0);
    // Enable muss für mind. 450 ns High bleiben, bevor es fallen darf!
    // ==> Also mind. 8 Zyklen warten
    _delay_us(1);
    set_output_bit(IO_DISPLAY_EN);
    _delay_us(1);
    // Alle Bits zurück setzen ==> Fallende Flanke von Enable
    clear_display_output_bits();
    _delay_us(50);
}


/*! 
 * Ein Zeichen auf das Display schreiben
 * @param data Das Zeichen
 */
void display_data(char data){ //ein Zeichen aus data in den Displayspeicher schreiben
    shift_data_out(data,SHIFT_LATCH, 0);
		
    // Enable muss für mind. 450 ns High bleiben, bevor es fallen darf!
    // ==> Also mind. 8 Zyklen warten
    set_output_bit(IO_DISPLAY_RS);
    _delay_us(1);
    set_output_bit(IO_DISPLAY_EN);
    _delay_us(1);
    // Alles zurück setzen ==> Fallende Flanke von Enable
    clear_display_output_bits();
    _delay_us(50);
}

/*
 * Löscht das ganze Display
 */
void display_clear(void){
    display_cmd(DISPLAY_CLEAR); // Display l�schen, Cursor Home
    _delay_ms(2);
}

/*!
 * Positioniert den Cursor
 * @param row Zeile
 * @param column Spalte
 */
void display_cursor (int row, int column) {
  switch (row) {
    case 2: column+=20;
    case 0:
      display_cmd (0x80 + column);
      break;
    case 3: column+=20;
    case 1:
      display_cmd (0xc0 + column);
      break;
  }
  _delay_us(150);
}

/*! 
 * Init Display
 */
void display_init(void){
  IO_DISPLAY_DDR |= DISPLAY_OUT;  // Ausgänge
  IO_DISPLAY_DDR &= ~DISPLAY_IN;  // Eingänge
  _delay_ms(20);                  // Display steht erst 10ms nach dem Booten bereit

  // Register in 8-Bit-Modus 3x Übertragen, dazwischen warten
  display_cmd(0x38);
  _delay_ms(5);
  display_cmd(0x38);
  _delay_ms(5);
  display_cmd(0x38);
  _delay_ms(5);
  display_cmd(0x14);
  _delay_ms(5);
  display_cmd(0x0e); //Display On, Cursor On, Cursor Blink
  display_clear();
}


/*!
 * Schreibt einen String auf das Display.
 * @param str auszugebene Zeichenkette
 * @param length Anzahl der Zeichen
 */
void display_out(char * str, uint8_t length){
  while(length--)
    display_data(*str++);
}

void display_print(char * str){
  while(*str)
    display_data(*str++);
}

#endif

#endif
