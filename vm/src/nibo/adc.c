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

/*! @file    adc.c
 *  @brief   Routinen zum Einlesen der Analogeingaenge
 *  @author  Benjamin Benz (bbe@heise.de)
 *  @author  modified by Nils Springob (nils@nicai-systems.de)
 *  @date    2007-01-26
 */

#include <avr/io.h>

#include "adc.h"

/*!
 * Initialisert den AD-Umsetzer. 
 * @param channel Für jeden Kanal, den man nutzen möchte, 
 * muss das entsprechende Bit in channel gesetzt sein
 * Bit0 = Kanal 0 usw.
 */
void adc_init(uint8_t channel){
  DDRA &= ~ channel;  // Pin als input
  PORTA &= ~ channel; // Alle Pullups aus.
}

/*!
 * Liest einen analogen Kanal aus
 * @param channel Kanal - hex-Wertigkeit des Pins (0x01 fuer PA0; 0x02 fuer PA1, ..)
 */
uint16_t adc_read(uint8_t channel){
  uint16_t result = 0x00;

  // Reference voltage: AVCC (5V)
  ADMUX= _BV(REFS0);

  ADMUX |= (channel & 0x07);  // Und jetzt Kanal waehlen, nur single ended

  ADCSRA = _BV(ADPS2)  // prescale faktor = 128 ADC laeuft
         | _BV(ADPS1)  // mit 14,7456MHz/ 128 = 115,2kHz
         | _BV(ADPS0)
         | _BV(ADEN)   // ADC an
         | _BV(ADSC);  // Beginne mit der Konvertierung

  // Warten bis konvertierung beendet, das sollte 25 ADC-Zyklen dauern,
  // also 1/4608 s
  while ( (ADCSRA & _BV(ADSC)) != 0) {
    asm volatile("nop");
  }

  // Ergebnis zusammenbauen
  result= ADCL;
  result+=(ADCH<<8);

  return result;
}
