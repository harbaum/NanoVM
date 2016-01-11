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

/**
 * @file   pwm.h
 * @brief  Funktionen für dir PWM-Signale und die Systemzeit
 * @author Nils Springob (nils@nicai-systems.de)
 * @date   2007-01-05
 */

#include "iodefs.h"


// PWM
extern uint16_t pwm_led_1; // 0 ... 1023
extern uint16_t pwm_led_2; // 0 ... 1023

// CLOCK
extern uint16_t pwm_timer_ms;  // zählt bis 1000(=1s) in 1ms Schritten
extern uint32_t pwm_timer_sec; // zählt die Sekunden seit Systemstart

void pwm_init(void);
