//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005 by Till Harbaum <Till@Harbaum.org>
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

//
//  native_lcd.c, native driver for Hitachi HD44780 based text LCDs
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef LCD

#include "stack.h"
#include "native.h"
#include "native_stdio.h"   // contains itoa
#include "native_lcd.h"

#include "delay.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// hitachi hd44780 controller commands
#define LCD_CLEAR           0x01
#define LCD_SET_ENTRY_MODE  0x04
#define LCD_ENTRY_INCR      0x02
#define LCD_ON              0x08
#define LCD_DISPLAY_ON      0x04
#define LCD_DISPLAY         0x08

#define LCD_FUNCTION        0x20
#define LCD_8_BIT           0x10
#define LCD_DOUBLE          0x08
#define LCD_SET_DISP_ADDR   0x80

#define LCD_BUSY            0x80

// disable LCD by asserting E
static inline void lcd_enable(void) {
  // enable LCD
  LCD_CTRL_PORT |=  _BV(LCD_CTRL_E); 
  delay(MICROSEC(10));
}

// disable LCD by deasserting E
static inline void lcd_disable(void) {
  LCD_CTRL_PORT &= ~_BV(LCD_CTRL_E); 
  delay(MICROSEC(10));
}

// switch LCD data port to input
static inline void lcd_data_input(void) {
  LCD_DATA_DDR  =  0x00;                  // data lines are input
  LCD_DATA_PORT =  0xff;                  // enable pull-ups
}

// access LCD by enabling and disabling E
static inline void lcd_write(u08_t data) {
  LCD_CTRL_PORT &= ~_BV(LCD_CTRL_RW);   // write

  LCD_DATA_DDR = 0xFF;       // set data I/O lines to output (8bit)
  LCD_DATA_PORT = data;      // output data, 8bits

  lcd_enable();
  lcd_disable();
  lcd_data_input();
}

static void lcd_wait_while_busy(void) {
  // wait until LCD busy bit goes to zero
  LCD_CTRL_PORT &= ~_BV(LCD_CTRL_RS); // ctrl access
  lcd_data_input();
  LCD_CTRL_PORT |=  _BV(LCD_CTRL_RW); // read

  lcd_enable();

  /* wait for BUSY flag to be gone */
  while(LCD_DATA_PIN & LCD_BUSY) {
    lcd_disable();
    lcd_enable();
  }

  lcd_disable();
}

// write the control byte to the display controller
static void lcd_write_ctrl(u08_t data) {
  lcd_wait_while_busy();

  LCD_CTRL_PORT &= ~_BV(LCD_CTRL_RS);  // ctrl access

  lcd_write(data);
}

// write a data byte to the display
static void lcd_write_data(u08_t data) {
  lcd_wait_while_busy();

  LCD_CTRL_PORT |=  _BV(LCD_CTRL_RS);   // data access

  lcd_write(data);
}

// send a string to the console and append return if ret is true
static void lcd_print(char *str) {
  u08_t chr;

#ifdef NVM_USE_EEPROM
  // check if source string is within internal nvm file, otherwise 
  // it's directly being read from ram
  if(NVMFILE_ISSET(str)) {
    while((chr = nvmfile_read08(str++)))
      lcd_write_data(chr);
  } else
#endif
    while(*str)
      lcd_write_data(*str++);
}

void native_lcd_init(void) {

  // make the three control lines outputs
  LCD_CTRL_PORT &= ~(_BV(LCD_CTRL_RS) | _BV(LCD_CTRL_RW) | _BV(LCD_CTRL_E));
  LCD_CTRL_DDR  |=  (_BV(LCD_CTRL_RS) | _BV(LCD_CTRL_RW) | _BV(LCD_CTRL_E));

  // don't drive data lines
  lcd_data_input();

  // writing three times the config byte makes sure, that 
  // the interface is in 8 bit mode afterwards
  lcd_write_ctrl(LCD_FUNCTION | LCD_8_BIT | LCD_DOUBLE);
  lcd_write_ctrl(LCD_FUNCTION | LCD_8_BIT | LCD_DOUBLE);
  lcd_write_ctrl(LCD_FUNCTION | LCD_8_BIT | LCD_DOUBLE);

  lcd_write_ctrl(LCD_ON | LCD_DISPLAY_ON);
  lcd_write_ctrl(LCD_SET_ENTRY_MODE | LCD_ENTRY_INCR);
}

void native_lcd_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_LCD_CLEAR) {
    lcd_write_ctrl(LCD_CLEAR);  // clear LCD
    // hitachi datasheet says 50 msec to wait before next command
    delay(MILLISEC(50));
  } else if(mref == NATIVE_METHOD_LCD_GOTOYX) {
    u08_t x = stack_pop();
    u08_t y = stack_pop();
    // set write address
    lcd_write_ctrl(LCD_SET_DISP_ADDR | ((y * LCD_VIRTUAL_LINE_LENGTH) + x));
  } else if(mref == NATIVE_METHOD_LCD_PRINT_STR) {
    lcd_print(stack_pop_addr());
  } else if(mref == NATIVE_METHOD_LCD_PRINT_INT) {
    char tmp[8];
    native_itoa((char*)tmp, stack_pop_int());
    lcd_print(tmp);
  } else if(mref == NATIVE_METHOD_LCD_PRINT_CHAR) {
    lcd_write_data(stack_pop_int());
  } else 
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

#endif
