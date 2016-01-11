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

/*! @file       pwm.c
 * @brief       Modul zur Erzeugung der Pwm-Signale und Zeitberechnung
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2007-01-05
*/


#include <avr/interrupt.h>
#include <stdlib.h> 
#include "ctbot/pwm.h"
#include "ctbot/pid.h"
#include "ctbot/wheelencoder.h"


#define PWM_SIGNAL_OC       SIG_OUTPUT_COMPARE0
#define PWM_TIMER_CNT       TCNT0
#define PWM_TIMER_CCR       TCCR0
#define PWM_TIMER_CCR_INIT  _BV(WGM01) | _BV(CS01) | _BV(CS00)
#define PWM_TIMER_OCR       OCR0
#define PWM_TIMER_OCR_INIT  9
#define PWM_TIMER_IMSK      _BV(OCIE0)


// diese Variablen enthalten die Soll-Werte: 
uint8_t pwm_servo_1;
uint8_t pwm_servo_2;
int16_t pwm_motor_l;
int16_t pwm_motor_r;

typedef struct
{
  uint8_t lo; // zählt bis 25(1ms) in 40us Schritten 
  uint8_t hi; // zählt bis 20(20ms) in 1ms Schritten
} pwm_cnt_t;

// interne Register
pwm_cnt_t pwm_servo_1_cnt;
pwm_cnt_t pwm_servo_2_cnt;
pwm_cnt_t pwm_motor_l_cnt;
pwm_cnt_t pwm_motor_r_cnt;

pwm_cnt_t pwm_timer_sub; // zählt bis 20*25(20ms) in 40us Schritten
uint16_t pwm_timer_ms;   // zählt bis 1000(1s) in 1ms Schritten
uint32_t pwm_timer_sec;  // zählt die Sekunden seit Systemstart

void pwm_init(void)
{
  pwm_timer_sub.lo = 25;
  pwm_timer_sub.hi = 0;
  pwm_timer_ms  = 0;
  pwm_timer_sec = 0;

  pwm_servo_1 = 0;
  pwm_servo_2 = 0;
  pwm_motor_l = 0;
  pwm_motor_r = 0;
  
  clear_output_bit(IO_MOTOR_L);
  clear_output_bit(IO_MOTOR_R);
  clear_output_bit(IO_SERVO_1);
  clear_output_bit(IO_SERVO_2);

  activate_output_bit(IO_MOTOR_L);
  activate_output_bit(IO_MOTOR_R);
  activate_output_bit(IO_SERVO_1);
  activate_output_bit(IO_SERVO_2);
  activate_output_bit(IO_MOTOR_L_DIR);
  activate_output_bit(IO_MOTOR_R_DIR);

  PWM_TIMER_CNT = 0;
  PWM_TIMER_CCR = PWM_TIMER_CCR_INIT;
  PWM_TIMER_OCR = PWM_TIMER_OCR_INIT;
  TIMSK |= PWM_TIMER_IMSK;  
}

SIGNAL(PWM_SIGNAL_OC)
//void do_pwm_int()
{
  if (pwm_timer_sub.lo==25)
  {
    // Jede Milisekunde:
    pwm_timer_sub.lo=0;
    ++pwm_timer_ms;
    if (pwm_timer_ms==1000)
    {
      pwm_timer_ms=0;
      ++pwm_timer_sec;
    }

    wheelencoder_isr();

    ++pwm_timer_sub.hi;
    // Setze die Bits wenn der Startwert erreicht wird:
    if (pwm_timer_sub.hi==20)
    {
      // LEFT MOTOR
      pwm_timer_sub.hi=0;
      pid_left_timer_int();
      pwm_motor_l_cnt.lo = abs(pwm_motor_l)%25;
      pwm_motor_l_cnt.hi = abs(pwm_motor_l)/25;
      if (pwm_motor_l_cnt.hi == 20)
        pwm_motor_l_cnt.hi = 21;
      set_output_bitval(IO_MOTOR_L_DIR, (pwm_motor_l>0));
      set_output_bitval(IO_MOTOR_L, (pwm_motor_l!=0));
    }

    if (pwm_timer_sub.hi==5)
    {
      // SERVO 1
      pwm_servo_1_cnt.lo = pwm_servo_1%25;
      pwm_servo_1_cnt.hi = 5+(pwm_servo_1/25)+1;
      set_output_bitval(IO_SERVO_1, (pwm_servo_1>0));
    }

    if (pwm_timer_sub.hi==10)
    {
      // RIGHT MOTOR
      pid_right_timer_int();
      pwm_motor_r_cnt.lo = abs(pwm_motor_r)%25;
      pwm_motor_r_cnt.hi = ((250+abs(pwm_motor_r))%500) / 25;
      if (abs(pwm_motor_r)==500)
        pwm_motor_r_cnt.hi = 21;
      set_output_bitval(IO_MOTOR_R_DIR, (pwm_motor_r<0));
      set_output_bitval(IO_MOTOR_R, (pwm_motor_r!=0));
    }

    if (pwm_timer_sub.hi==15)
    {
      // SERVO 2
      pwm_servo_2_cnt.lo = pwm_servo_2%25;
      pwm_servo_2_cnt.hi = 15+(pwm_servo_2/25)+1;
      set_output_bitval(IO_SERVO_2, (pwm_servo_2>0));
    }
  }

  uint8_t next = 25 - pwm_timer_sub.lo;
 
  if (pwm_motor_l_cnt.hi==pwm_timer_sub.hi)
  {
    int8_t diff = pwm_motor_l_cnt.lo-pwm_timer_sub.lo; 
    if (diff>0)
    {
      if (diff<next)
         next=diff;
    }
    else if (diff==0)
    {
      clear_output_bit(IO_MOTOR_L);
    }
  }

  if (pwm_motor_r_cnt.hi==pwm_timer_sub.hi)
  {
    int8_t diff = pwm_motor_r_cnt.lo-pwm_timer_sub.lo;
    if (diff>0)
    {
      if (diff<next)
         next=diff;
    }
    else if (diff==0)
    {
      clear_output_bit(IO_MOTOR_R);
    }
  }

  if (pwm_servo_1_cnt.hi==pwm_timer_sub.hi)
  {
    int8_t diff = pwm_servo_1_cnt.lo-pwm_timer_sub.lo;
    if (diff>0)
    {
      if (diff<next)
         next=diff;
    }
    else if (diff==0)
    {
      clear_output_bit(IO_SERVO_1);
    }
  }

  if (pwm_servo_2_cnt.hi==pwm_timer_sub.hi)
  {
    int8_t diff = pwm_servo_2_cnt.lo-pwm_timer_sub.lo;
    if (diff>0)
    {
      if (diff<next)
         next=diff;
    }
    else if (diff==0)
    {
      clear_output_bit(IO_SERVO_2);
    }
  }

  pwm_timer_sub.lo += next;
  PWM_TIMER_OCR=next*10-1; 
}


