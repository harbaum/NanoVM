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
//  native_display.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "stack.h"
#include "nibo/native.h"
#include "nibo/native_graphicdisplay.h"
#include "nibo/display.h"
#include "nibo/delay.h"
#include "nibo/gfx.h"

#define NATIVE_METHOD_clear 1
#define NATIVE_METHOD_gotoXY 2
#define NATIVE_METHOD_print 3
#define NATIVE_METHOD_setProportional 4
#define NATIVE_METHOD_getProportional 5

uint8_t proportional;

void native_nibo_graphicdisplay_init(void) {
  display_init();
  display_wait(0x00);
  display_write(0x00, 0x3f);
  display_wait(0x00);
  display_write(0x00, 0xc0);
  display_wait(0x00);
  display_write(0x00, 0x40);
  display_wait(0x00);
  display_write(0x00, 0xb8);

  display_wait(0x01);
  display_write(0x01, 0x3f);
  display_wait(0x01);
  display_write(0x01, 0xc0);
  display_wait(0x01);
  display_write(0x01, 0x40);
  display_wait(0x01);
  display_write(0x01, 0xb8);

  gfx_fill(0x00);

  
  gfx_move(40, 0);
  gfx_print_text("nibo 1.0", 1);
  gfx_move(5, 12);
  gfx_print_text("(c) 2007 by nicai-systems", 1);

  uint8_t prop = 0;

//  gfx_move(15, 16);
//  gfx_print_text(" !\"#$%&\\()*+,-./", prop);
  gfx_move(15, 24);
  gfx_print_text("0123456789:;<=>?", prop);
  gfx_move(15, 32);
  gfx_print_text("@ABCDEFGHIJKLMNO", prop);
  gfx_move(15, 40);
  gfx_print_text("PQRSTUVWXYZ[\\]^_", prop);
  gfx_move(15, 48);
  gfx_print_text("`abcdefghijklmno", prop);
  gfx_move(15, 56);
  gfx_print_text("pqrstuvwxyz{|}~", prop);
  gfx_print_char(127, prop);
  
/*
  for (int16_t i=0; i<64; ++i)
  {
    display_wait(0x02);
    display_write(0x02, i);
    //display_wait(0x03);
    //display_write(0x03, i);
  }
  
  display_wait(0x00);
  display_write(0x00, 0xb9);
  for (int16_t i=0; i<64; ++i)
  {
    display_wait(0x02);
    display_write(0x02, i);
    //display_wait(0x03);
    //display_write(0x03, i);
  }
  */
  
}



void native_nibo_graphicdisplay_invoke(u08_t mref) {

  if(mref == NATIVE_METHOD_clear) {
    gfx_fill(0x00);  // clear LCD

  } else if(mref == NATIVE_METHOD_gotoXY) {
    u08_t y = stack_pop();
    u08_t x = stack_pop();
    gfx_move(x, y);

  } else if(mref == NATIVE_METHOD_print) {
    char * str = stack_pop_addr();
#ifdef NVM_USE_EEPROM
    if(NVMFILE_ISSET(str)) {
      char chr;
      while((chr = nvmfile_read08(str++)))
        gfx_print_char(chr, proportional);
    } else
#endif
      while(*str)
        gfx_print_char(*str++, proportional);
        
  } else if(mref == NATIVE_METHOD_setProportional) {
    proportional=stack_pop()?1:0;
    
  } else if(mref == NATIVE_METHOD_getProportional) {
    nvm_int_t val = proportional;
    stack_push(val);
    
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

