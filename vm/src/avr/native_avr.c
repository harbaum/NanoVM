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
//  native_avr.c
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifndef CTBOT
#ifndef NIBO
#ifdef AVR


#include "vm.h"
#include "native.h"
#include "native_avr.h"
#include "stack.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined(ATMEGA128)
volatile u08_t *ports[] = { &PORTA, &PORTB, &PORTC, &PORTD, &PORTE, &PORTF };
volatile u08_t *ddrs[]  = { &DDRA,  &DDRB,  &DDRC,  &DDRD,  &DDRE,  &DDRF  };
volatile u08_t *pins[]  = { &PINA,  &PINB,  &PINC,  &PIND,  &PINE,  &PINF  };

#elif defined(ATMEGA32)
volatile u08_t *ports[] = { &PORTA, &PORTB, &PORTC, &PORTD };
volatile u08_t *ddrs[]  = { &DDRA,  &DDRB,  &DDRC,  &DDRD  };
volatile u08_t *pins[]  = { &PINA,  &PINB,  &PINC,  &PIND  };

#elif defined(ATMEGA8)
volatile u08_t *ports[] = { NULL,  &PORTB, &PORTC, &PORTD  };
volatile u08_t *ddrs[]  = { NULL,   &DDRB,  &DDRC,  &DDRD  };
volatile u08_t *pins[]  = { NULL,   &PINB,  &PINC,  &PIND  };

#elif defined(ATMEGA168)
volatile u08_t *ports[] = { NULL,  &PORTB, &PORTC, &PORTD  };
volatile u08_t *ddrs[]  = { NULL,   &DDRB,  &DDRC,  &DDRD  };
volatile u08_t *pins[]  = { NULL,   &PINB,  &PINC,  &PIND  };

#else
#error "Unsupported AVR CPU!"
#endif

volatile static nvm_int_t ticks;

SIGNAL(SIG_OUTPUT_COMPARE1A) {
  TCNT1 = 0;
  ticks++;
}

void native_init(void) {
  // init timer
  TCCR1B = _BV(CS11);           // clk/8
  OCR1A = (u16_t)(CLOCK/800u);  // 100 Hz is default
#if defined(ATMEGA168)
  TIMSK1 |= _BV(OCIE1A);         // interrupt on compare
#else
  TIMSK |= _BV(OCIE1A);         // interrupt on compare
#endif

}

// the AVR class
void native_avr_avr_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_GETCLOCK) {
    stack_push(CLOCK/1000);
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

// the port class
void native_avr_port_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_SETINPUT) {
    u08_t bit  = stack_pop();
    u08_t port = stack_pop();
    DEBUGF("native setinput %bd/%bd\n", port, bit);
    *ddrs[port] &= ~_BV(bit);
  } else if(mref == NATIVE_METHOD_SETOUTPUT) {
    u08_t bit  = stack_pop();
    u08_t port = stack_pop();
    DEBUGF("native setoutput %bd/%bd\n", port, bit);
    *ddrs[port] |= _BV(bit);
  } else if(mref == NATIVE_METHOD_SETBIT) {
    u08_t bit  = stack_pop();
    u08_t port = stack_pop();
    DEBUGF("native setbit %bd/%bd\n", port, bit);
    *ports[port] |= _BV(bit);
  } else if(mref == NATIVE_METHOD_CLRBIT) {
    u08_t bit  = stack_pop();
    u08_t port = stack_pop();
    DEBUGF("native clrbit %bd/%bd\n", port, bit);
    *ports[port] &= ~_BV(bit);
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

// the timer class, based on AVR 16 bit timer 1
void native_avr_timer_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_SETSPEED) {
    OCR1A = stack_pop_int();  // set reload value
  } else if(mref == NATIVE_METHOD_GET) {
    stack_push(ticks);
  } else if(mref == NATIVE_METHOD_TWAIT) {
    nvm_int_t wait = stack_pop_int();
    ticks = 0;
    while(ticks < wait);      // reset watchdog here if enabled
  } else if(mref == NATIVE_METHOD_SETPRESCALER) {
    TCCR1B = stack_pop_int();
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

// the Adc class
void native_avr_adc_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_ADC_SETPRESCALER) {
    ADCSRA = _BV(ADEN) | (stack_pop_int() & 7);  // set prescaler value
  } else if(mref == NATIVE_METHOD_ADC_SETREFERENCE) {
    ADMUX = stack_pop_int() & 0xc0;              // set reference value
  } else if(mref == NATIVE_METHOD_ADC_GETVALUE) {
    // ADLAR = 0
    ADMUX = (ADMUX & 0xc0) | (stack_pop_int() & 0x0f);

    // do conversion
    ADCSRA |= _BV(ADSC);                  // Start conversion
    while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
    ADCSRA |= _BV(ADIF);                  // clear ADCIF
    stack_push(ADCL + (ADCH << 8));
  } else if(mref == NATIVE_METHOD_ADC_GETBYTE) {
    // ADLAR = 1
    ADMUX = (ADMUX & 0xc0) | _BV(ADLAR) | (stack_pop_int() & 0x0f);

    // do conversion
    ADCSRA |= _BV(ADSC);                  // Start conversion
    while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
    ADCSRA |= _BV(ADIF);                  // clear ADCIF
    stack_push(ADCH);
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

// the Pwm class
#if (!defined(ATMEGA8) && !defined(ATMEGA168) && !defined(ATMEGA32))
#error Please add PWM support for your CPU!
#endif

void native_avr_pwm_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_PWM_SETPRESCALER) {
    u08_t value = stack_pop();
    u08_t port = stack_pop();   // top byte contains class ref

    // implement pwm1
    if(port == 1) {
      // keep everything but prescaler
#if defined(ATMEGA168)
      TCCR2B = (TCCR2B & ~7) | (value & 7);
#else
      TCCR2 = (TCCR2 & ~7) | (value & 7);
#endif
    }

  } else if(mref == NATIVE_METHOD_PWM_SETRATIO) {
    u08_t value = stack_pop();
    u08_t port = stack_pop();   // top byte contains class ref

    // implement pwm1
    if(port == 1) {

      // setup of timer 2: keep prescaler, set fast pwm
      // and clear OC2 on match, set OC2 on top
#if defined(ATMEGA168)
      TCCR2A = _BV(WGM21) | _BV(WGM20);
      TCCR2B = (TCCR2B & 7) | _BV(COM2A1);
#else
      TCCR2 = (TCCR2 & 7) | _BV(WGM21) | _BV(WGM20) | _BV(COM21);
#endif

#if defined(ATMEGA8)
      // the pwm output is on pin b.3 on the mega8
      DDRB |= _BV(3);        // OC2 -> output
#elif defined(ATMEGA168)
      // the pwm output is on pin b.3 on the mega8
      DDRB |= _BV(3);        // OC2 -> output
#elif defined(ATMEGA32)
      // the pwm output is on pin d.7 on the mega32
      DDRD |= _BV(7);        // OC2 -> output
#elif defined(ATMEGA128)
      // the pwm output is on pin b.7 on the mega128
      DDRB |= _BV(7);        // OC2 -> output
#else
#error Please add PWM support for your CPU!
#endif

#if defined(ATMEGA168)
      OCR2A = value;
#else
      OCR2 = value;
#endif
    }
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

#endif

#endif // !NIBO
#endif // !CTBOT
