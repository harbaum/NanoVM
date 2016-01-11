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
//  native_edgetetector.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "nibo/native.h"
#include "nibo/native_edgedetector.h"
#include "nibo/iodefs.h"

#include "nibo/adc.h"

#define NATIVE_METHOD_update 1
#define NATIVE_METHOD_getLeftRel 2
#define NATIVE_METHOD_getRightRel 3
#define NATIVE_METHOD_getLeftAbs 4
#define NATIVE_METHOD_getRightAbs 5

uint16_t edge_value_off_l;
uint16_t edge_value_off_r;
uint16_t edge_value_on_l;
uint16_t edge_value_on_r;

void native_nibo_edgedetector_init(void) {
  clear_output_bit(IO_LINE_FLOOR_EN);
  activate_output_bit(IO_LINE_FLOOR_EN);
  adc_init(AN_FLOOR_L);
  adc_init(AN_FLOOR_R);
}

// the ctbot class
void native_nibo_edgedetector_invoke(u08_t mref) {

  // JAVA: void update()
  if(mref == NATIVE_METHOD_update) {
    clear_output_bit(IO_LINE_FLOOR_EN);
    delay_us(25);
    edge_value_off_l = 1023-adc_read(AN_FLOOR_L);
    edge_value_off_r = 1023-adc_read(AN_FLOOR_R);
    edge_value_off_r += 1023-adc_read(AN_FLOOR_R);
    edge_value_off_l += 1023-adc_read(AN_FLOOR_L);
    set_output_bit(IO_LINE_FLOOR_EN);
    delay_us(25);
    edge_value_on_l = 1023-adc_read(AN_FLOOR_L);
    edge_value_on_r = 1023-adc_read(AN_FLOOR_R);
    edge_value_on_r += 1023-adc_read(AN_FLOOR_R);
    edge_value_on_l += 1023-adc_read(AN_FLOOR_L);
    clear_output_bit(IO_LINE_FLOOR_EN);
  }
  
  // JAVA: int getLeftRel()
  else if(mref == NATIVE_METHOD_getLeftRel) {
    nvm_int_t val = edge_value_on_l-edge_value_off_l;
    stack_push(val);
  }

  // JAVA: int getRightRel()
  else if(mref == NATIVE_METHOD_getRightRel) {
    nvm_int_t val = edge_value_on_r-edge_value_off_r;
    stack_push(val);
  }

  // JAVA: int getLeftAbs()
  else if(mref == NATIVE_METHOD_getLeftAbs) {
    nvm_int_t val = edge_value_on_l;
    stack_push(val);
  }

  // JAVA: int getRightAbs()
  else if(mref == NATIVE_METHOD_getRightAbs) {
    nvm_int_t val = edge_value_on_r;
    stack_push(val);
  }

  // ERROR
  else
    error(ERROR_NATIVE_UNKNOWN_METHOD);

}

