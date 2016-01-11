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
//  native_distancesensor.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "nibo/native.h"
#include "nibo/native_distancesensor.h"
#include "nibo/iodefs.h"

#include "distco.h"

#define NATIVE_METHOD_update 1
#define NATIVE_METHOD_getSensor 2
#define NATIVE_METHOD_setEnabled 3
#define NATIVE_METHOD_getEnabled 4

uint8_t enabled;

void native_nibo_distsensor_init(void) {
}

void native_nibo_distsensor_invoke(u08_t mref) {

  // JAVA: void update()
  if(mref == NATIVE_METHOD_update) {
    distco_update();
  }
  
  // JAVA: int getSensor(int id)
  else if(mref == NATIVE_METHOD_getSensor) {
    nvm_int_t id = stack_pop_int();
    if ((id<0) || (id>4))
      error(ERROR_NATIVE_ILLEGAL_ARGUMENT);
    nvm_int_t val = distco_distance[id];
    stack_push(val);
  }

  // JAVA: void setEnabled(boolean enabled)
  else if(mref == NATIVE_METHOD_setEnabled) {
    if (stack_pop_int()) {
      enabled=1;
      distco_startMeasure();
    } else {
      enabled=0;
      distco_stop();
    }
  }

  // JAVA: boolean getEnabled()
  else if(mref == NATIVE_METHOD_getEnabled) {
    stack_push(enabled);
  }

  // ERROR
  else {
    error(ERROR_NATIVE_UNKNOWN_METHOD);
  }
}

