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
//  native_display.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "stack.h"
#include "ctbot/native.h"
#include "ctbot/native_display.h"
#include "ctbot/display.h"
#include "ctbot/delay.h"

#define NATIVE_METHOD_clear 1
#define NATIVE_METHOD_gotoXY 2
#define NATIVE_METHOD_print 3
#define NATIVE_METHOD_setCursorMode 4

void native_ctbot_display_init(void) {
  display_init();
  display_cursor(1, 0);
  display_print("  Welcome to Java   ");
  display_cursor(2, 0);
  display_print("    NanoVM          ");
  display_cursor(2, 11);
  display_print(VERSION);
  delay(1500);
  display_clear();
}



void native_ctbot_display_invoke(u08_t mref) {

  if(mref == NATIVE_METHOD_clear) {
    display_clear();  // clear LCD

  } else if(mref == NATIVE_METHOD_gotoXY) {
    u08_t y = stack_pop();
    u08_t x = stack_pop();
    display_cursor(y, x);

  } else if(mref == NATIVE_METHOD_print) {
    char * str = stack_pop_addr();
#ifdef NVM_USE_EEPROM
    if(NVMFILE_ISSET(str)) {
      char chr;
      while((chr = nvmfile_read08(str++)))
        display_data(chr);
    } else
#endif
      while(*str)
        display_data(*str++);
  } else if(mref == NATIVE_METHOD_setCursorMode) {
    stack_pop_addr(); //TODO: implement

  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

