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
// native.h
//

#ifndef NATIVE_H
#define NATIVE_H

#define NATIVE_CLASS_BASE           16

// java/lang/Object
#define NATIVE_CLASS_OBJECT         (NATIVE_CLASS_BASE+0)
#define NATIVE_METHOD_INIT          0

// java/lang/System
#define NATIVE_CLASS_SYSTEM         (NATIVE_CLASS_BASE+1)
#define NATIVE_FIELD_OUT            0
#define NATIVE_FIELD_IN             1

// java/io/PrintStream
#define NATIVE_CLASS_PRINTSTREAM    (NATIVE_CLASS_BASE+2)
#define NATIVE_METHOD_PRINTLN_STR   1
#define NATIVE_METHOD_PRINTLN_INT   2
#define NATIVE_METHOD_PRINTLN_CHAR  3
#define NATIVE_METHOD_PRINT_STR     4
#define NATIVE_METHOD_PRINT_INT     5
#define NATIVE_METHOD_PRINT_CHAR    6
#define NATIVE_METHOD_FORMAT        7

// java/io/InputStream
#define NATIVE_CLASS_INPUTSTREAM    (NATIVE_CLASS_BASE+3)
#define NATIVE_METHOD_INPUTSTREAM_AVAILABLE 1
#define NATIVE_METHOD_INPUTSTREAM_READ      2

// java/lang/StringBuffer
#define NATIVE_CLASS_STRINGBUFFER   (NATIVE_CLASS_BASE+4)
#define NATIVE_METHOD_INIT_STR      1
#define NATIVE_METHOD_APPEND_STR    2
#define NATIVE_METHOD_APPEND_INT    3
#define NATIVE_METHOD_APPEND_CHR    4
#define NATIVE_METHOD_TOSTRING      5
#define NATIVE_METHOD_APPEND_FLOAT  6

// nanovm/avr/AVR
#define NATIVE_CLASS_AVR            (NATIVE_CLASS_BASE+5)
#define NATIVE_METHOD_GETCLOCK      1
#define NATIVE_FIELD_PORTA          0
#define NATIVE_FIELD_PORTB          1
#define NATIVE_FIELD_PORTC          2
#define NATIVE_FIELD_PORTD          3
#define NATIVE_FIELD_PORTE          4
#define NATIVE_FIELD_PORTF          5
#define NATIVE_FIELD_PORTG          6
#define NATIVE_FIELD_PORTH          7

// nanovm/avr/Port
#define NATIVE_CLASS_PORT           (NATIVE_CLASS_BASE+6)
#define NATIVE_METHOD_SETINPUT      1
#define NATIVE_METHOD_SETOUTPUT     2
#define NATIVE_METHOD_SETBIT        3
#define NATIVE_METHOD_CLRBIT        4

// nanovm/avr/Timer
#define NATIVE_CLASS_TIMER          (NATIVE_CLASS_BASE+7)
#define NATIVE_METHOD_SETSPEED      1
#define NATIVE_METHOD_GET           2
#define NATIVE_METHOD_TWAIT         3
#define NATIVE_METHOD_SETPRESCALER  4

// nanovm/avr/Uart (use PrintStream/InputStream for io)
#define NATIVE_CLASS_UART           (NATIVE_CLASS_BASE+8)

// nanovm/avr/Pwm
#define NATIVE_CLASS_PWM            (NATIVE_CLASS_BASE+9)
#define NATIVE_METHOD_PWM_SETPRESCALER  1
#define NATIVE_METHOD_PWM_SETRATIO      2
#define NATIVE_FIELD_PWM0               0
#define NATIVE_FIELD_PWM1               1

// nanovm/avr/Adc
#define NATIVE_CLASS_ADC            (NATIVE_CLASS_BASE+10)
#define NATIVE_METHOD_ADC_SETPRESCALER  1
#define NATIVE_METHOD_ADC_SETREFERENCE  2
#define NATIVE_METHOD_ADC_GETVALUE      3
#define NATIVE_METHOD_ADC_GETBYTE       4

// nanovm/Lcd
#define NATIVE_CLASS_LCD            (NATIVE_CLASS_BASE+11)
#define NATIVE_METHOD_LCD_CLEAR         1
#define NATIVE_METHOD_LCD_GOTOYX        2
#define NATIVE_METHOD_LCD_PRINT_STR     3
#define NATIVE_METHOD_LCD_PRINT_INT     4
#define NATIVE_METHOD_LCD_PRINT_CHAR    5

// nanovm/ctbot/drivers/*
#define NATIVE_CLASS_CTBOT_BOT            (NATIVE_CLASS_BASE+12)
#define NATIVE_CLASS_CTBOT_CLOCK          (NATIVE_CLASS_BASE+13)
#define NATIVE_CLASS_CTBOT_DISPLAY        (NATIVE_CLASS_BASE+14)
#define NATIVE_CLASS_CTBOT_DISTANCESENSOR (NATIVE_CLASS_BASE+15)
#define NATIVE_CLASS_CTBOT_EDGEDETECTOR   (NATIVE_CLASS_BASE+16)
#define NATIVE_CLASS_CTBOT_IRRECEIVER     (NATIVE_CLASS_BASE+17)
#define NATIVE_CLASS_CTBOT_LDRSENSOR      (NATIVE_CLASS_BASE+18)
#define NATIVE_CLASS_CTBOT_LEDS           (NATIVE_CLASS_BASE+19)
#define NATIVE_CLASS_CTBOT_LIGHTBARRIER   (NATIVE_CLASS_BASE+20)
#define NATIVE_CLASS_CTBOT_LINEDETECTOR   (NATIVE_CLASS_BASE+21)
#define NATIVE_CLASS_CTBOT_MOTOR          (NATIVE_CLASS_BASE+22)
#define NATIVE_CLASS_CTBOT_MOUSE          (NATIVE_CLASS_BASE+23)
#define NATIVE_CLASS_CTBOT_SERVO          (NATIVE_CLASS_BASE+24)
#define NATIVE_CLASS_CTBOT_SHUTTERSENSOR  (NATIVE_CLASS_BASE+25)
#define NATIVE_CLASS_CTBOT_WHEELENCODER   (NATIVE_CLASS_BASE+26)

// nanovm/lang/*
#define NATIVE_CLASS_MATH           (NATIVE_CLASS_BASE+27)

// nanovm/util/Formatter
#define NATIVE_CLASS_FORMATTER      (NATIVE_CLASS_BASE+28)


#define NATIVE_ID(c,m)  ((c<<8)|m)

#define NATIVE_ID2CLASS(m)   (m>>8)
#define NATIVE_ID2METHOD(m)  (m&0xff)
#define NATIVE_ID2FIELD(m)   (m&0xff)


#endif // NATIVE_H
