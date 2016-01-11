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

/*! @file       pid.h
 * @brief       Modul zur PID-Regelung der Motoren
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2007-01-05
*/

#ifndef _CTBOT_PID_
#define _CTBOT_PID_

// Soll Werte für DIP Controller:
extern int16_t pid_left_value;
extern int16_t pid_right_value;

// Ist Werte der PID Controller:
extern int16_t pid_left_speed;
extern int16_t pid_right_speed;


// Konfiguration:
extern uint8_t pid_mode;
#define PID_LEFT_ENABLED 0x01
#define PID_RIGHT_ENABLED 0x02

// These values will be incremented by wheel encoders
extern int8_t pid_left_cnt;
extern int8_t pid_right_cnt;

void pid_reset(void);
void pid_left_timer_int(void);
void pid_right_timer_int(void);

#endif
