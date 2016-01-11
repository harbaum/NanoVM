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
//  native_leds.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef NIBO

#include "nibo/delay.h"
#include "stack.h"
#include "vm.h"
#include "nibo/native.h"
#include "nibo/native_leds.h"
#include "nibo/display.h"
#include "nibo/delay.h"
#include "nibo/gfx.h"
#include "nibo/pwm.h"

#include "nibo/iodefs.h"
#include "nibo/i2c_master.h"
#include "avrisp.h"
#include "distco.h"
#include "motco.h"
#include "nibo/adc.h"

#define NATIVE_METHOD_setStatus 1
#define NATIVE_METHOD_getStatus 2
#define NATIVE_METHOD_setHeadlights 3
#define NATIVE_METHOD_getHeadlights 4
#define NATIVE_METHOD_setDisplaylight 5
#define NATIVE_METHOD_getDisplaylight 6

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

int16_t tspeed_l = 0;
int16_t tspeed_r = 0;

void native_nibo_leds_init(void) {
  activate_output_bit(IO_LED_WHITE);
  activate_output_bit(IO_DISP_LIGHT);
  IO_LEDS_RED_DDR = IO_LEDS_RED_MASK;
  IO_LEDS_GREEN_DDR = IO_LEDS_GREEN_MASK;
}

// the ctbot class
void native_nibo_leds_invoke(u08_t mref) {

  // JAVA: void setStatus(int state)
  if(mref == NATIVE_METHOD_setStatus) {
    nvm_int_t color = stack_pop_int();
    nvm_int_t led = stack_pop_int();
    if (color&0x01)
      IO_LEDS_GREEN_PORT |= 0x04<<led;
    else
      IO_LEDS_GREEN_PORT &= ~(0x04<<led);
    
    if (color&0x02)
      IO_LEDS_RED_PORT |= 0x04<<led;
    else
      IO_LEDS_RED_PORT &= ~(0x04<<led);
  }
  
  // JAVA: int getStatus()
  else if(mref == NATIVE_METHOD_getStatus) {
    nvm_int_t led = stack_pop_int();
    stack_push(led);
  }
  
  // JAVA: void setHeadlights(int state)
  else if(mref == NATIVE_METHOD_setHeadlights) {
    nvm_int_t light = stack_pop_int();
    pwm_led_1 = light;
  }
  
  // JAVA: int getHeadlights()
  else if(mref == NATIVE_METHOD_getHeadlights) {
    nvm_int_t light = pwm_led_1;
    stack_push(light);
  }
  
  // JAVA: void setDisplaylight(int state)
  else if(mref == NATIVE_METHOD_setDisplaylight) {
    nvm_int_t light = stack_pop_int();
    pwm_led_2 = light;
  }
  
  // JAVA: int getDisplaylight()
  else if(mref == NATIVE_METHOD_getDisplaylight) {
    nvm_int_t light = pwm_led_2;
    stack_push(light);
  
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);

}

#endif
