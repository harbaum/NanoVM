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
//  native_irreceiver.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "nibo/native.h"
#include "nibo/native_irtransceiver.h"
#include "nibo/iodefs.h"
#include "nibo/distco.h"

//#include "nibo/ir_rc5.h"

#define NATIVE_METHOD_getCommand 1
#define NATIVE_METHOD_sendCommand 2

void native_nibo_irtransceiver_init(void) {
 // ir_init();
}

// the ctbot class
void native_nibo_irtransceiver_invoke(u08_t mref) {

  // JAVA: int getCommand()
  if(mref == NATIVE_METHOD_getCommand) {
    distco_update();
    nvm_int_t val = distco_rc5_cmd;
    distco_rc5_cmd = 0;
    stack_push(val);
  }
    
  // JAVA: void sendCommand(int)
  else if(mref == NATIVE_METHOD_sendCommand) {
    nvm_int_t val = stack_pop_int();
    distco_transmitRC5(val);
  }
   
  // ERROR
  else {
    error(ERROR_NATIVE_UNKNOWN_METHOD);
  }
}

