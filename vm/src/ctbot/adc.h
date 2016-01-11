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

/*! @file    adc.h
 *  @brief   Routinen zum Einlesen der Analogeingaenge
 *  @author  Benjamin Benz (bbe@heise.de)
 *  @author  modified by Nils Springob (nils@nicai-systems.de)
 *  @date    2007-01-26
 */

#ifndef ADC_H_
#define ADC_H_

/*!
 * Liest einen analogen Kanal aus
 * @param channel Kanal - hex-Wertigkeit des Pins (0x01 fuer PA0; 0x02 fuer PA1, ..)
 */
uint16_t adc_read(uint8_t channel);

/*!
 * Initialisert den AD-Umsetzer. 
 * @param channel fuer jeden Kanal, den man nutzen moechte, 
 * muss das entsprechende Bit in channel gesetzt sein.
 * Bit0 = Kanal 0 usw.
 */
void adc_init(uint8_t channel);

#endif
