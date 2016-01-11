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
//  native_mouse.c
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
#include "ctbot/native_mouse.h"
#include "ctbot/mouse.h"

#include <avr/interrupt.h>

#define NATIVE_METHOD_getX 1
#define NATIVE_METHOD_getY 2
#define NATIVE_METHOD_getQuality 3
#define NATIVE_METHOD_requestPicture 4
#define NATIVE_METHOD_getPixel 5
#define NATIVE_METHOD_reset 6

void native_ctbot_mouse_init(void) {
  maus_sens_init();
}

// the ctbot class
void native_ctbot_mouse_invoke(u08_t mref) {

  // JAVA: int getX()
  if(mref == NATIVE_METHOD_getX) {
    nvm_int_t val = (int8_t)maus_sens_read(MOUSE_DELTA_X_REG);
    stack_push(val);
  }

  // JAVA: int getY()
  else if(mref == NATIVE_METHOD_getY) {
    nvm_int_t val = (int8_t)maus_sens_read(MOUSE_DELTA_Y_REG);
    stack_push(val);
  }

  // JAVA: int getQuality()
  else if(mref == NATIVE_METHOD_getQuality) {
    nvm_int_t val = maus_sens_read(MOUSE_SQUAL_REG);
    stack_push(val);
  }

  // JAVA: void requestPicture()
  else if(mref == NATIVE_METHOD_requestPicture) {
    maus_image_prepare();
  }

  // JAVA: int getPixel()
  else if(mref == NATIVE_METHOD_getPixel) {
    nvm_int_t val = maus_image_read();
    stack_push(val);
  }

  // JAVA: void reset()
  else if(mref == NATIVE_METHOD_reset) {
    maus_sens_write(MOUSE_CONFIG_REG,MOUSE_CFG_RESET);       //Reset sensor
    maus_sens_write(MOUSE_CONFIG_REG,MOUSE_CFG_FORCEAWAKE);  //Always on

  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);

}

#endif
