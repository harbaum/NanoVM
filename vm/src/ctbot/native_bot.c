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
//  native_bot.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_bot.h"
#include "ctbot/iodefs.h"

#include "ctbot/shift.h"
#include "ctbot/adc.h"

#define NATIVE_METHOD_getError 1
#define NATIVE_METHOD_setExtension1Enabled 2
#define NATIVE_METHOD_getExtension1Enabled 3
#define NATIVE_METHOD_setExtension2Enabled 4
#define NATIVE_METHOD_getExtension2Enabled 5


void native_ctbot_bot_init(void) {
  deactivate_output_bit(IO_SENS_ERROR);
}

// the ctbot class
void native_ctbot_bot_invoke(u08_t mref) {

  // JAVA: boolean getError()
  if(mref == NATIVE_METHOD_getError) {
    nvm_int_t val = get_input_bit(IO_SENS_ERROR)?1:0;
    stack_push(val);
  }
  
  // JAVA: void setExtension1Enabled()
  else if(mref == NATIVE_METHOD_setExtension1Enabled) {
    if (stack_pop_int())
      shift_reg_ena &= ~(uint8_t)ENA_ERW1;
    else
      shift_reg_ena |= ENA_ERW1;
    shift_data(shift_reg_ena, SHIFT_REGISTER_ENA);
  }
  
  // JAVA: int getExtension1Enabled()
  else if(mref == NATIVE_METHOD_getExtension1Enabled) {
    nvm_int_t val = (shift_reg_ena & ENA_ERW1)?1:0;
    stack_push(val);
  }

  // JAVA: int setExtension2Enabled()
  else if(mref == NATIVE_METHOD_setExtension2Enabled) {
    if (stack_pop_int())
      shift_reg_ena &= ~(uint8_t)ENA_ERW2;
    else
      shift_reg_ena |= ENA_ERW2;
    shift_data(shift_reg_ena, SHIFT_REGISTER_ENA);
  }

  // JAVA: int getExtension2Enabled()
  else if(mref == NATIVE_METHOD_getExtension2Enabled) {
    nvm_int_t val = (shift_reg_ena & ENA_ERW2)?1:0;
    stack_push(val);
  }

}

