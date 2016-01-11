/*
 * c't-Bot
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version. 
 * This program is distributed in the hope that it will be 
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the Free 
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 * 
 */

/*! @file   shift.c
 *  @brief  Routinen zur Ansteuerung der Shift-Register
 *  @author Benjamin Benz (bbe@heise.de)
 *  @author modified by Nils Springob to work with nanoVM
 *  @date   2007-01-26
 */

#include <stdint.h>
#include "ct-Bot.h"

#include "shift.h"

#include <avr/io.h>
#ifdef NEW_AVR_LIB
# include <util/delay.h>
#else
# include <avr/delay.h>
#endif


#define SHIFT_OUT   0x1F   /*!< Alle Pins die Ausgänge sind */
#define SHIFT_PORT  PORTC  /*!< Port an dem die Register haengen */
#define SHIFT_DDR   DDRC   /*!< DDR des Ports an dem die Register haengen */

uint8_t shift_reg_led;
uint8_t shift_reg_ena;

/*!
 * Initialisert die Shift-Register
 */
void shift_init(){
  SHIFT_DDR |= SHIFT_OUT;		// Ausgänge Schalten
  SHIFT_PORT &= ~SHIFT_OUT; 	// Und auf Null
}

/*!
 * Schiebt Daten durch eines der drei 74HC595 Schieberegister
 * Achtung den Port sollte man danach noch per shift_clear() zurücksetzen
 * @param data	Das Datenbyte
 * @param latch_data der Pin an dem der Daten-latch-Pin des Registers (PIN 11) hängt
 * @param latch_store der Pin an dem der latch-Pin zum transfer des Registers (PIN 12) hängt
 */
void shift_data_out(uint8_t data, uint8_t latch_data, uint8_t latch_store){
	int8_t i;

	SHIFT_PORT &= ~SHIFT_OUT;		// und wieder clear	
	for (i=8; i>0; i--){
		SHIFT_PORT |= ((data >> 7)& 0x01);      // Das oberste Bit von data auf PC0.0 (Datenleitung Schieberegister)
		SHIFT_PORT |= latch_data ;	    		// und ins jeweilige Storageregister latchen
		data= data << 1;		      	// data links schieben
		SHIFT_PORT &= ~SHIFT_OUT;		// und wieder clear
	}
	
	SHIFT_PORT |= latch_store;			// alles vom Storage ins Output register latchen
}

/*!
 * Schiebt Daten durch eines der drei 74HC595 Schieberegister
 * vereinfachte Version, braucht kein shift_clear()
 * geht NICHT für das Shift-register, an dem das Display-hängt!!!
 * @param data	Das Datenbyte
 * @param latch_data der Pin an dem der Daten-latch-Pin des Registers (PIN 11) hängt
 */
void shift_data(uint8_t data, uint8_t latch_data){
	shift_data_out(data, latch_data, SHIFT_LATCH);
	shift_clear();
}

/*!
 * Setzt die Shift-Register wieder zurück
 */ 
void shift_clear(){
	SHIFT_PORT &= ~SHIFT_OUT;		// und wieder clear	
}
