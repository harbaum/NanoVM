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

/*! @file   wheelencoder.h
 *  @brief  Low-Level Routinen fuer die Sensor-Steuerung des c't-Bots
 *  @author Benjamin Benz (bbe@heise.de)
 *  @author modified by Nils Springob (nils@nicai-systems.de)
 *  @date   2007-01-08
 */
#ifndef _WHEELENCODER_H_
#define _WHEELENCODER_H_

extern uint16_t wheelencoder_l;
extern uint16_t wheelencoder_r;


/*!
 * Kuemmert sich um die Radencoder
 * Das muss schneller gehen als die anderen Sensoren,
 * daher Update per ISR
 */
extern void wheelencoder_isr(void);

extern void wheelencoder_init(void);

#endif
