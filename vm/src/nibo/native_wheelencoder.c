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
#include "nibo/native.h"
#include "nibo/native_wheelencoder.h"
#include "nibo/iodefs.h"
#include "motco.h"

#define NATIVE_METHOD_getLeftInc 1
#define NATIVE_METHOD_getRightInc 2
#define NATIVE_METHOD_setEnabled 3
#define NATIVE_METHOD_getEnabled 4
#define NATIVE_METHOD_getLeftSpeed 5
#define NATIVE_METHOD_getRightSpeed 6
#define NATIVE_METHOD_update 7

int16_t last_ticks_l;
int16_t last_ticks_r;


void native_nibo_wheelencoder_init(void) {
}

void native_nibo_wheelencoder_invoke(u08_t mref) {

  // JAVA: void update()
  if(mref == NATIVE_METHOD_update) {
    motco_update();
  }
  
  // JAVA: int getLeftInc()
  else if(mref == NATIVE_METHOD_getLeftInc) {
    nvm_int_t val = (int16_t)(motco_ticks_l-last_ticks_l);
    last_ticks_l = motco_ticks_l;
    stack_push(val);
  }

  // JAVA: int getRightInc()
  else if(mref == NATIVE_METHOD_getRightInc) {
    nvm_int_t val = (int16_t)(motco_ticks_r-last_ticks_r);
    last_ticks_r = motco_ticks_r;
    stack_push(val);
  }

  // JAVA: void setEnabled(boolean enabled)
  else if(mref == NATIVE_METHOD_setEnabled) {
    stack_pop_int();
  }

  // JAVA: boolean getEnabled()
  else if(mref == NATIVE_METHOD_getEnabled) {
    stack_push(1);
  }

  // JAVA: int getLeftSpeed()
  else if(mref == NATIVE_METHOD_getLeftSpeed) {
    nvm_int_t val = motco_speed_l;
    stack_push(val);
  }

  // JAVA: int getRightSpeed()
  else if(mref == NATIVE_METHOD_getRightSpeed) {
    nvm_int_t val = motco_speed_r;
    stack_push(val);
  
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
  
}

