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
//  native_ldrsensor.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_ldrsensor.h"
#include "ctbot/iodefs.h"

#include "ctbot/shift.h"
#include "ctbot/adc.h"

#define NATIVE_METHOD_updateLeft 1
#define NATIVE_METHOD_updateRight 2
#define NATIVE_METHOD_getLeft 3
#define NATIVE_METHOD_getRight 4

uint16_t value_l;
uint16_t value_r;

void native_ctbot_ldrsensor_init(void) {
  adc_init(SENS_LDR_L);
  adc_init(SENS_LDR_R);
}

// the ctbot class
void native_ctbot_ldrsensor_invoke(u08_t mref) {

  // JAVA: void updateLeft()
  if(mref == NATIVE_METHOD_updateLeft) {
    value_l = adc_read(SENS_LDR_L);
  }
  
  // JAVA: void updateRight()
  else if(mref == NATIVE_METHOD_updateRight) {
    value_r = adc_read(SENS_LDR_R);
  }
  
  // JAVA: int getLeft()
  else if(mref == NATIVE_METHOD_getLeft) {
    nvm_int_t val = value_l;
    stack_push(val);
  }

  // JAVA: int getRight()
  else if(mref == NATIVE_METHOD_getRight) {
    nvm_int_t val = value_r;
    stack_push(val);
  }

}

