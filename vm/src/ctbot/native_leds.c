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
//  native_leds.c
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
#include "ctbot/native_leds.h"

#include "ctbot/shift.h"

#define NATIVE_METHOD_set 1
#define NATIVE_METHOD_get 2

void native_ctbot_leds_init(void) {
  shift_init();
  shift_data(0xc0, SHIFT_REGISTER_LED);
  delay_ms(10);
  shift_data(0x00, SHIFT_REGISTER_LED);
}

// the ctbot class
void native_ctbot_leds_invoke(u08_t mref) {
  static nvm_int_t leds = 0;
  // JAVA: void set(int state)
  if(mref == NATIVE_METHOD_set) {
    leds = stack_pop_int();
    shift_data(leds,SHIFT_REGISTER_LED);
  }
  
  // JAVA: int get()
  else if(mref == NATIVE_METHOD_get) {
    stack_push(leds);
  }
}

#endif
