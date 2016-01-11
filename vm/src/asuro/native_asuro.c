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
//  native_asuro.c
//
//  native java vm routines for dlr asuro robot
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef ASURO

#include "delay.h"
#include "stack.h"
#include "vm.h"
#include "native.h"
#include "native_asuro.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// make 36khz pwm for ir data transmission
#define TIMER2_RELOAD  (CLOCK/(72000))

static u08_t ticks;
static volatile u16_t msec;

#if defined(ATMEGA168)
SIGNAL(SIG_OUTPUT_COMPARE2A) {
#else
SIGNAL(SIG_OUTPUT_COMPARE2) {
#endif
  if(ticks++ == 72) {
    ticks = 0;
    msec++;
  }
}

#define FWD     (_BV(PB5))
#define RWD     (_BV(PB4))
#define BREAK   (0x00)
#define FREE    (_BV(PB4) | _BV(PB5))

#define IRTX        _BV(PB3)
#define GREEN_LED   _BV(PB0)
#define RED_LED     _BV(PD2)

#define PWM       (_BV(PB1) | _BV(PB2))
#define RIGHT_DIR (_BV(PB4) | _BV(PB5))
#define LEFT_DIR  (_BV(PD4) | _BV(PD5))

#define SWITCH_PULLUP   _BV(PD3)

#define ADC_BATTERIE    _BV(MUX0) | _BV(MUX2) //ADC5
#define ADC_SWITCHES    _BV(MUX2)             //ADC4
#define ADC_LINE_LEFT   _BV(MUX0) | _BV(MUX1) //ADC3
#define ADC_LINE_RIGHT  _BV(MUX1)             //ADC2
#define ADC_WHEEL_LEFT  _BV(MUX0)             //ADC1
#define ADC_WHEEL_RIGHT 0                     //ADC0

#define FRONT_LED _BV(PD6)

#define MOTORSENSOR_LED     (7)
#define MOTORSENSOR_LED_ON  PORTD |=  _BV(MOTORSENSOR_LED)
#define MOTORSENSOR_LED_OFF PORTD &= ~_BV(MOTORSENSOR_LED)


// initialize the asuro hardware
void native_init(void) {
  // configure all outputs
  DDRB = IRTX | LEFT_DIR | PWM | GREEN_LED;
  DDRD = RIGHT_DIR | FRONT_LED | _BV(MOTORSENSOR_LED) | RED_LED;

#if defined(ATMEGA168)
  // toggle on compare, clk/1
  TCCR2A = _BV(WGM21) | _BV(COM2A0);
  TCCR2B = _BV(CS20);
  // 36kHz carrier/timer
  OCR2A = TIMER2_RELOAD;
  // interrupt on compare
  TIMSK2 |= _BV(OCIE2A);
#else
  // toggle on compare, clk/1
  TCCR2 = _BV(WGM21) | _BV(CS20) | _BV(COM20);
  // 36kHz carrier/timer
  OCR2 = TIMER2_RELOAD;
  // interrupt on compare
  TIMSK |= _BV(OCIE2);
#endif

  // for PWM (8-Bit PWM) on OC1A & OC1B
  TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
  // tmr1 running on MCU clock/8
  TCCR1B = _BV(CS11);

  // A/D Conversion
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1); // clk/64
  MOTORSENSOR_LED_OFF;

  // backleds off
  PORTD &= ~_BV(7);            // Wheel LED OFF
  DDRC  |= _BV(0) | _BV(1);    // Output => no motor sensors
  PORTC &= ~(_BV(0) | _BV(1)); // Both back leds off

  sei();
}

// the asuro class
void native_asuro_invoke(u08_t mref) {

  // JAVA: void statusLED(int state)
  if(mref == NATIVE_METHOD_STATUSLED) {
    nvm_int_t status = stack_pop();

    if(status & 1) PORTD |=  _BV(2);
    else           PORTD &= ~_BV(2);

    if(status & 2) PORTB |=  _BV(0);
    else           PORTB &= ~_BV(0);
  }

  // JAVA: void wait(int msec)
  else if(mref == NATIVE_METHOD_WAIT) {
    nvm_int_t time = stack_pop();

    msec = 0;
    while(msec < time);
  }

  // JAVA: void motor(int left, int right)
  else if(mref == NATIVE_METHOD_MOTOR) {
    nvm_int_t right = stack_pop_int();
    nvm_int_t left = stack_pop_int();

    PORTB &= ~(_BV(PB4) | _BV(PB5));  // stop
    if(right < 0) {
      PORTB |= RWD;
      right = -right;
    } else if(right > 0)
      PORTB |= FWD;

    PORTD &= ~(_BV(PB4) | _BV(PB5));  // stop
    if(left < 0) {
      PORTD |= RWD;
      left = -left;
    } else if(left > 0)
      PORTD |= FWD;

    if(left  >  255) left  =  255;
    if(right >  255) right =  255;

    // set pwm
    OCR1A = left;
    OCR1B = right;
  }

  // JAVA: void lineLED(int state)
  else if(mref == NATIVE_METHOD_LINELED) {
    nvm_int_t status = stack_pop();
    if(status) PORTD |=  _BV(6);
    else       PORTD &= ~_BV(6);
  }

  // JAVA: void backLED(int left, int right)
  else if(mref == NATIVE_METHOD_BACKLED) {
    nvm_int_t right = stack_pop();
    nvm_int_t left = stack_pop();

    if (left || right) {
      PORTD &= ~_BV(7);         // Wheel LED OFF
      DDRC  |= _BV(0) | _BV(1); // Output => no odometrie
      PORTC |= _BV(0) | _BV(1);
    }

    if(!left)  PORTC &= ~_BV(1);
    if(!right) PORTC &= ~_BV(0);
  }

  // JAVA: int lineSensor(int sensor)
  else if(mref == NATIVE_METHOD_LINESENSOR) {
    if(stack_pop())   // AVCC reference with external capacitor
      ADMUX = _BV(REFS0) | ADC_LINE_RIGHT;
    else
      ADMUX = _BV(REFS0) | ADC_LINE_LEFT;

    ADCSRA |= _BV(ADSC);                  // Start conversion
    while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
    ADCSRA |= _BV(ADIF);                  // clear ADCIF
    stack_push(ADCL + (ADCH << 8));
  }

  // JAVA: int motorSensor(int sensor)
  else if(mref == NATIVE_METHOD_MOTORSENSOR) {
    DDRC &= ~(_BV(0) | _BV(1)); // Input => no break LED
    MOTORSENSOR_LED_ON;

    if(stack_pop())   // AVCC reference with external capacitor
      ADMUX = _BV(REFS0) | ADC_WHEEL_RIGHT;
    else
      ADMUX = _BV(REFS0) | ADC_WHEEL_LEFT;

    ADCSRA |= _BV(ADSC);                  // Start conversion
    while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
    ADCSRA |= (1 << ADIF);                // clear ADCIF
    stack_push(ADCL + (ADCH << 8));
  }

  // JAVA: int getSwitches(int mode)
  else if(mref == NATIVE_METHOD_GETSWITCHES) {
    u16_t i;

    if(stack_pop()) {
      // get via analog interface
      DDRD  |= SWITCH_PULLUP;               // Switch pullup as Output
      PORTD |= SWITCH_PULLUP;
      ADMUX = _BV(REFS0) | ADC_SWITCHES;
      delay(MICROSEC(200));

      ADCSRA |= _BV(ADSC);                  // Start conversion
      while(!(ADCSRA & _BV(ADIF)));         // wait for conversion complete
      ADCSRA |= (1 << ADIF);                // clear ADCIF
      i = ADCL + (ADCH << 8);
      PORTD &= ~SWITCH_PULLUP;

      stack_push(((64512 - i/2)/i)-62);
    } else {
      DDRD  &= ~SWITCH_PULLUP;              // Switch pullup as input
      delay(MICROSEC(200));
      stack_push((PIND & SWITCH_PULLUP)?0:1);
    }
  }
}

#endif
