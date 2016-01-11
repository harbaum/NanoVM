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
//  native_impl.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "vm.h"
#include "nvmfile.h"
#include "native_impl.h"
#include "ctbot/native.h"
#include "ctbot/shift.h"
#include "ctbot/delay.h"

#include "stack.h"

#ifdef NVM_USE_STDIO
#include "native_stdio.h"
#endif

#ifdef NVM_USE_MATH
#include "native_math.h"
#endif

#ifdef NVM_USE_FORMATTER
#include "native_formatter.h"
#endif


#include "ctbot/native_bot.h"
#include "ctbot/native_clock.h"
#include "ctbot/native_display.h"
#include "ctbot/native_distancesensor.h"
#include "ctbot/native_edgedetector.h"
#include "ctbot/native_ldrsensor.h"
#include "ctbot/native_leds.h"
#include "ctbot/native_linedetector.h"
#include "ctbot/native_lightbarrier.h"
#include "ctbot/native_motor.h"
#include "ctbot/native_mouse.h"
#include "ctbot/native_servo.h"
#include "ctbot/native_shuttersensor.h"
#include "ctbot/native_wheelencoder.h"
#include "ctbot/native_irreceiver.h"


void native_java_lang_object_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_INIT) {
    /* ignore object constructor ... */
    stack_pop();  // pop object reference
  } else 
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}
  
void native_new(u16_t mref) {
  if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_STRINGBUFFER) {
    // create empty stringbuf object and push reference onto stack
    stack_push(NVM_TYPE_HEAP | heap_alloc(FALSE, 1));
  } else 
    error(ERROR_NATIVE_UNKNOWN_CLASS);
}

void native_invoke(u16_t mref) {
  // check for native classes/methods
  if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_OBJECT) {
    native_java_lang_object_invoke(NATIVE_ID2METHOD(mref));

#ifdef NVM_USE_STDIO
  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_PRINTSTREAM) {
    native_java_io_printstream_invoke(NATIVE_ID2METHOD(mref));
  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_INPUTSTREAM) {
    native_java_io_inputstream_invoke(NATIVE_ID2METHOD(mref));
  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_STRINGBUFFER) {
    native_java_lang_stringbuffer_invoke(NATIVE_ID2METHOD(mref));
#endif

#ifdef NVM_USE_MATH
  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_MATH) {
    native_math_invoke(NATIVE_ID2METHOD(mref));
#endif

#ifdef NVM_USE_FORMATTER
    // the formatter class
  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_FORMATTER) {
    native_formatter_invoke(NATIVE_ID2METHOD(mref));
#endif

   // the c't-Bot specific classes

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_BOT) {
    native_ctbot_bot_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_CLOCK) {
    native_ctbot_clock_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_DISPLAY) {
    native_ctbot_display_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_DISTANCESENSOR) {
    native_ctbot_distsensor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_EDGEDETECTOR) {
    native_ctbot_edgedetector_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_IRRECEIVER) {
    native_ctbot_irreceiver_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_LDRSENSOR) {
    native_ctbot_ldrsensor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_LEDS) {
    native_ctbot_leds_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_LINEDETECTOR) {
    native_ctbot_linedetector_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_LIGHTBARRIER) {
    native_ctbot_lightbarrier_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_MOTOR) {
    native_ctbot_motor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_MOUSE) {
    native_ctbot_mouse_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_SERVO) {
    native_ctbot_servo_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_SHUTTERSENSOR) {
    native_ctbot_shuttersensor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_CTBOT_WHEELENCODER) {
    native_ctbot_wheelencoder_invoke(NATIVE_ID2METHOD(mref));

  } else {
    error(ERROR_NATIVE_UNKNOWN_CLASS);
  }
}

void native_init(void)
{
    native_ctbot_clock_init();
    native_ctbot_leds_init();
    native_ctbot_display_init();
    // here the other inits will start:
    native_ctbot_bot_init();
    native_ctbot_distsensor_init();
    native_ctbot_edgedetector_init();
    native_ctbot_irreceiver_init();
    native_ctbot_ldrsensor_init();
    native_ctbot_lightbarrier_init();
    native_ctbot_linedetector_init();
    native_ctbot_motor_init();
    native_ctbot_mouse_init();
    native_ctbot_servo_init();
    native_ctbot_shuttersensor_init();
    native_ctbot_wheelencoder_init();

    // show init complete:
    shift_data(0x03, SHIFT_REGISTER_LED);
    delay_ms(10);
    shift_data(0x00, SHIFT_REGISTER_LED);
}

