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

//
//  native_motor.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef CTBOT

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_motor.h"
#include "ctbot/pwm.h"
#include "ctbot/pid.h"
#include "ctbot/display.h"

#include <avr/interrupt.h>

#define NATIVE_METHOD_setLeft 1
#define NATIVE_METHOD_setRight 2
#define NATIVE_METHOD_getLeft 3
#define NATIVE_METHOD_getRight 4
#define NATIVE_METHOD_setLeftSpeed 5
#define NATIVE_METHOD_setRightSpeed 6
#define NATIVE_METHOD_getLeftSpeed 7
#define NATIVE_METHOD_getRightSpeed 8
#define NATIVE_METHOD_stop 9

void native_ctbot_motor_init(void) {
  //pwm_init(); already done by clock!
  //display_cursor(2,1);
  //display_print("motor loaded   ");
}

// the ctbot class
void native_ctbot_motor_invoke(u08_t mref) {

  // JAVA: void setLeft(int val)
  if(mref == NATIVE_METHOD_setLeft) {
    nvm_int_t val = stack_pop_int();
    cli();
    pwm_motor_l=val;
    pid_mode &= ~PID_LEFT_ENABLED;
    sei();
  }

  // JAVA: void setRight(int val)
  else if(mref == NATIVE_METHOD_setRight) {
    nvm_int_t val = stack_pop_int();
    cli();
    pwm_motor_r=val;
    pid_mode &= ~PID_RIGHT_ENABLED;
    sei();
  }
  
  else if(mref == NATIVE_METHOD_setLeftSpeed) {
    nvm_int_t val = stack_pop_int();
    cli();
    pid_left_value=val;
    pid_mode |= PID_LEFT_ENABLED;
    sei();
  }

  // JAVA: void setRight(int val)
  else if(mref == NATIVE_METHOD_setRightSpeed) {
    nvm_int_t val = stack_pop_int();
    cli();
    pid_right_value=val;
    pid_mode |= PID_RIGHT_ENABLED;
    sei();
  }

  // JAVA: int getLeft()
  else if(mref == NATIVE_METHOD_getLeft) {
    nvm_int_t val = pwm_motor_l;
    stack_push(val);
  }

  // JAVA: int getRight()
  else if(mref == NATIVE_METHOD_getRight) {
    nvm_int_t val = pwm_motor_r;
    stack_push(val);
  }

  // JAVA: int getLeft()
  else if(mref == NATIVE_METHOD_getLeftSpeed) {
    nvm_int_t val = pwm_motor_l;
    stack_push(val);
  }

  // JAVA: int getRight()
  else if(mref == NATIVE_METHOD_getRightSpeed) {
    nvm_int_t val = pwm_motor_r;
    stack_push(val);
  }

  // JAVA: void stopt()
  else if(mref == NATIVE_METHOD_stop) {
    cli();
    pwm_motor_r = 0;
    pwm_motor_l = 0;
    pid_mode &= ~(PID_RIGHT_ENABLED|PID_LEFT_ENABLED);
    pid_reset();
    sei();
  }

}

#endif
