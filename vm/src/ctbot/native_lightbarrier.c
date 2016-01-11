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
//  native_lightbarrier.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_lightbarrier.h"
#include "ctbot/iodefs.h"

#include "ctbot/shift.h"
#include "ctbot/adc.h"

#define NATIVE_METHOD_getState 1
#define NATIVE_METHOD_setEnabled 2
#define NATIVE_METHOD_getEnabled 3

void native_ctbot_lightbarrier_init(void) {
  deactivate_output_bit(IO_SENS_SCHRANKE);
}

// the ctbot class
void native_ctbot_lightbarrier_invoke(u08_t mref) {

  // JAVA: void getState()
  if(mref == NATIVE_METHOD_getState) {
    nvm_int_t val = get_input_bit(IO_SENS_SCHRANKE)?1:0;
    stack_push(val);
  }
  
  // JAVA: void setEnabled()
  else if(mref == NATIVE_METHOD_setEnabled) {
    if (stack_pop_int())
      shift_reg_ena &= ~(uint8_t)ENA_SCHRANKE;
    else
      shift_reg_ena |= ENA_SCHRANKE;
    shift_data(shift_reg_ena, SHIFT_REGISTER_ENA);
  }

  // JAVA: boolean getEnabled()
  else if(mref == NATIVE_METHOD_getEnabled) {
    nvm_int_t val = (shift_reg_ena & ENA_SCHRANKE)?1:0;
    stack_push(val);
  }
}

