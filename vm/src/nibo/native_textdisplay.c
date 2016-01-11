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
#include "nibo/native_textdisplay.h"
#include "nibo/display.h"
#include "nibo/delay.h"
#include "nibo/gfx.h"

#define NATIVE_METHOD_clear 1
#define NATIVE_METHOD_gotoXY 2
#define NATIVE_METHOD_print 3
#define NATIVE_METHOD_setCursorMode 4

uint8_t proportional;

u08_t cursor_x;
u08_t cursor_y;

void print_char(char c) {
  if ((cursor_x>15)||(cursor_y>1))
    return;
  display_wait(0x00);
  display_write(0x02, c);
  cursor_x++;
  if ((cursor_x==16)&&(cursor_y==0))
  {
    cursor_x=0;
    cursor_y=1;
    display_wait(0x00);
    display_write(0x00, 0x80+0x40);
  }
}


void native_nibo_textdisplay_init(void) {
  display_init();
  //clear display
  display_wait(0x00);
  display_write(0x00, 0x01);
  //display on, blink cursor
  display_wait(0x00);
  display_write(0x00, 0x0f);
  // 8 bit mode, 2-line, 5x7 font
  display_wait(0x00);
  display_write(0x00, 0x38);
}



void native_nibo_textdisplay_invoke(u08_t mref) {

  // JAVA: void clear()
  if(mref == NATIVE_METHOD_clear) {
    display_wait(0x00);
    display_write(0x00, 0x01);
  
  // JAVA: void gotoXY(int x, int y)
  } else if(mref == NATIVE_METHOD_gotoXY) {
    cursor_y = stack_pop();
    cursor_x = stack_pop();

    if ((cursor_x>15)||(cursor_y>1))
      return;
      
    display_wait(0x00);
    if (cursor_y==0)
      display_write(0x00, 0x80+0x00+cursor_x);
    else
      display_write(0x00, 0x80+0x40+cursor_x);

  // JAVA: void print(String str)
  } else if(mref == NATIVE_METHOD_print) {
    char * str = stack_pop_addr();
#ifdef NVM_USE_EEPROM
    if(NVMFILE_ISSET(str)) {
      char chr;
      while((chr = nvmfile_read08(str++)))
        print_char(chr);
    } else
#endif
      while(*str)
        print_char(*str++);

  // JAVA: void setCursorMode(int mode)
  } else if(mref == NATIVE_METHOD_setCursorMode) {
    uint8_t mode = stack_pop();
    display_wait(0x00);
    if (mode==0)
      display_write(0x00, 0x0c);
    else if (mode==1)
      display_write(0x00, 0x0e);
    else if (mode==2)
      display_write(0x00, 0x0f);
    
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

