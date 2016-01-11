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
//  native_asuro.c
//
//  native java vm routines for dlr asuro robot
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
#include "ctbot/native_servo.h"
#include "ctbot/pwm.h"
#include "ctbot/display.h"

#include <avr/interrupt.h>

#define NATIVE_METHOD_set1 1
#define NATIVE_METHOD_set2 2
#define NATIVE_METHOD_get1 3
#define NATIVE_METHOD_get2 4

void native_ctbot_servo_init(void) {
  //pwm_init(); already done by clock!
  //display_cursor(3,1);
  //display_print("servo loaded   ");
  
}

// the ctbot class
void native_ctbot_servo_invoke(u08_t mref) {

  // JAVA: void set1(int val)
  if(mref == NATIVE_METHOD_set1) {
    nvm_int_t val = stack_pop_int();
    cli();
    pwm_servo_1=val;
    sei();
  }

  // JAVA: void set2(int val)
  else if(mref == NATIVE_METHOD_set2) {
    nvm_int_t val = stack_pop_int();
    cli();
    pwm_servo_2=val;
    sei();
  }

  // JAVA: int get1()
  else if(mref == NATIVE_METHOD_get1) {
    nvm_int_t val = pwm_servo_1;
    stack_push(val);
  }

  // JAVA: int get2()
  else if(mref == NATIVE_METHOD_get2) {
    nvm_int_t val = pwm_servo_2;
    stack_push(val);
  }

}

#endif
