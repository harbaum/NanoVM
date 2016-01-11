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
//  native_wheelencoder.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_wheelencoder.h"
#include "ctbot/iodefs.h"

#include "ctbot/shift.h"
#include "ctbot/wheelencoder.h"
#include "ctbot/pid.h"

#define NATIVE_METHOD_getLeftInc 1
#define NATIVE_METHOD_getRightInc 2
#define NATIVE_METHOD_setEnabled 3
#define NATIVE_METHOD_getEnabled 4
#define NATIVE_METHOD_getLeftSpeed 5
#define NATIVE_METHOD_getRightSpeed 6

void native_ctbot_wheelencoder_init(void) {
  wheelencoder_init();
  // shift was initialized by LED's
}

// the ctbot class
void native_ctbot_wheelencoder_invoke(u08_t mref) {

  // JAVA: int getLeftInc()
  if(mref == NATIVE_METHOD_getLeftInc) {
    cli();
    nvm_int_t val = (int16_t)wheelencoder_l;
    wheelencoder_l = 0;
    sei();
    stack_push(val);
  }

  // JAVA: int getRightInc()
  else if(mref == NATIVE_METHOD_getRightInc) {
    cli();
    nvm_int_t val = (int16_t)wheelencoder_r;
    wheelencoder_r = 0;
    sei();
    stack_push(val);
  }

  // JAVA: void setEnabled(boolean enabled)
  else if(mref == NATIVE_METHOD_setEnabled) {
    if (stack_pop_int())
      shift_reg_ena &= ~(uint8_t)ENA_RADLED;
    else
      shift_reg_ena |= ENA_RADLED;
    shift_data(shift_reg_ena, SHIFT_REGISTER_ENA);
  }

  // JAVA: boolean getEnabled()
  else if(mref == NATIVE_METHOD_getEnabled) {
    stack_push((shift_reg_ena & ENA_RADLED)?1:0);
  }

  // JAVA: int getLeftSpeed()
  else if(mref == NATIVE_METHOD_getLeftSpeed) {
    cli();
    nvm_int_t val = pid_left_speed;
    sei();
    stack_push(val);
  }

  // JAVA: int getRightSpeed()
  else if(mref == NATIVE_METHOD_getRightSpeed) {
    cli();
    nvm_int_t val = pid_right_speed;
    sei();
    stack_push(val);
  }
  
}

