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
//  native_impl.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#include "vm.h"
#include "nvmfile.h"
#include "nibo/native.h"
#include "native_impl.h"
#include "nibo/delay.h"

#include <avr/interrupt.h>

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


#include "nibo/native_bot.h"
#include "nibo/native_clock.h"
#include "nibo/native_graphicdisplay.h"
#include "nibo/native_distancesensor.h"
#include "nibo/native_edgedetector.h"
#include "nibo/native_leds.h"
#include "nibo/native_linedetector.h"
#include "nibo/native_motor.h"
#include "nibo/native_wheelencoder.h"
#include "nibo/native_irtransceiver.h"


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

   // the Nibo specific classes

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_BOT) {
    native_nibo_bot_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_CLOCK) {
    native_nibo_clock_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_GRAPHICDISPLAY) {
    native_nibo_graphicdisplay_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_DISTANCESENSOR) {
    native_nibo_distsensor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_EDGEDETECTOR) {
    native_nibo_edgedetector_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_IRTRANSCEIVER) {
    native_nibo_irtransceiver_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_LEDS) {
    native_nibo_leds_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_LINEDETECTOR) {
    native_nibo_linedetector_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_MOTOR) {
    native_nibo_motor_invoke(NATIVE_ID2METHOD(mref));

  } else if(NATIVE_ID2CLASS(mref) == NATIVE_CLASS_NIBO_WHEELENCODER) {
    native_nibo_wheelencoder_invoke(NATIVE_ID2METHOD(mref));

  } else {
    error(ERROR_NATIVE_UNKNOWN_CLASS);
  }
}

void native_init(void)
{
  // Basic initialization:
  native_nibo_bot_init();
    
  // initialize other classes:
  native_nibo_clock_init();
  native_nibo_graphicdisplay_init();
  native_nibo_leds_init();
  native_nibo_distsensor_init();
  native_nibo_edgedetector_init();
  native_nibo_irtransceiver_init();
  native_nibo_linedetector_init();
  native_nibo_motor_init();
  native_nibo_wheelencoder_init();
  sei();
}

