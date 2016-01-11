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

/*! @file       pwm.c
 * @brief       Modul zur Erzeugung der Pwm-Signale und Zeitberechnung
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2007-01-05
*/


#include <avr/interrupt.h>
#include <stdlib.h> 
#include "nibo/pwm.h"


#define PWM_SIGNAL_OV       SIG_OVERFLOW1
#define PWM_TIMER_CNT       TCNT1

#define PWM_TIMER_CCRA      TCCR1A
#define PWM_TIMER_CCRA_INIT _BV(WGM11) | _BV(COM1A1) | _BV(COM1B1)
#define PWM_TIMER_CCRB      TCCR1B
#define PWM_TIMER_CCRB_INIT  _BV(CS11) | _BV(WGM13)

#define PWM_TIMER_OCRA      OCR1A
#define PWM_TIMER_OCRB      OCR1B
#define PWM_TIMER_IMSK      _BV(TOIE1)
#define PWM_TIMER_ICR       ICR1
#define PWM_TIMER_ICR_INIT  1024


/*
Der Timer läuft mit einer Frequenz von 2 MHz und zählt bis 2048. Dadurch wird
die Interruptroutine alle 1.024 ms aufgerufen.
*/


// diese Variablen enthalten die Soll-Werte: 
uint16_t pwm_led_1;
uint16_t pwm_led_2;

uint16_t pwm_timer_sub;  // zählt bis 1000 (1ms) in 1us Schritten
uint16_t pwm_timer_ms;   // zählt bis 1000 (1s) in 1ms Schritten
uint32_t pwm_timer_sec;  // zählt die Sekunden seit Systemstart

void pwm_init(void)
{
  pwm_timer_sub = 0;
  pwm_timer_ms  = 0;
  pwm_timer_sec = 0;

  pwm_led_1 = 0;
  pwm_led_2 = 0;

  clear_output_bit(IO_LED_WHITE);
  clear_output_bit(IO_DISP_LIGHT);

  activate_output_bit(IO_LED_WHITE);
  activate_output_bit(IO_DISP_LIGHT);

  PWM_TIMER_CNT = 0;
  PWM_TIMER_CCRA = PWM_TIMER_CCRA_INIT;
  PWM_TIMER_CCRB = PWM_TIMER_CCRB_INIT;
  PWM_TIMER_ICR = PWM_TIMER_ICR_INIT;
  TIMSK |= PWM_TIMER_IMSK;
}

SIGNAL(PWM_SIGNAL_OV)
{
  pwm_timer_sub+=24;
  if (pwm_timer_sub>=1000){
    pwm_timer_sub-=1000;
    pwm_timer_ms++;
  }
  pwm_timer_ms++;

  if (pwm_timer_ms>=1000){
    pwm_timer_ms-=1000;
    pwm_timer_sec++;
  }
  if (pwm_led_1>=1023)
    PWM_TIMER_OCRA = 1024;
  else
    PWM_TIMER_OCRA = pwm_led_1;

  if (pwm_led_2>=1023)
    PWM_TIMER_OCRB = 1024;
  else
    PWM_TIMER_OCRB = pwm_led_2;
}
