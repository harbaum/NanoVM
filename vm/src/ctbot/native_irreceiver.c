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
//  native_irreceiver.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "ctbot/native.h"
#include "ctbot/native_irreceiver.h"
#include "ctbot/iodefs.h"

#include "ctbot/ir_rc5.h"

#define NATIVE_METHOD_getCommand 1

void native_ctbot_irreceiver_init(void) {
  ir_init();
}

// the ctbot class
void native_ctbot_irreceiver_invoke(u08_t mref) {

  // JAVA: int getLeft()
  if(mref == NATIVE_METHOD_getCommand) {
    cli();
    nvm_int_t val = ir_read();
    sei();
    stack_push(val);
  }
}

