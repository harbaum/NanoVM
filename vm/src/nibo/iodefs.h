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

#include <avr/io.h>

/*! red LEDs */
#define IO_LEDS_RED_PORT  PORTE
#define IO_LEDS_RED_MASK  0xfc
#define IO_LEDS_RED_DDR   DDRE

/*! green LEDs */
#define IO_LEDS_GREEN_PORT  PORTC
#define IO_LEDS_GREEN_MASK  0xfc
#define IO_LEDS_GREEN_DDR   DDRC

/*! white LED pair */
#define IO_LED_WHITE_PORT  PORTB
#define IO_LED_WHITE_BIT   5
#define IO_LED_WHITE_DDR   DDRB

/*! display illumination */
#define IO_DISP_LIGHT_PORT  PORTB
#define IO_DISP_LIGHT_BIT   6
#define IO_DISP_LIGHT_DDR   DDRB



/*! line and floor sensor */
#define IO_LINE_FLOOR_EN IO_LINE_FLOOR_EN
#define IO_LINE_FLOOR_EN_PORT  PORTG
#define IO_LINE_FLOOR_EN_BIT   0
#define IO_LINE_FLOOR_EN_DDR   DDRG


/*! analog channels */
#define AN_FLOOR_R    0        /*!< ADC-PIN floorsensor right */
#define AN_FLOOR_L    1        /*!< ADC-PIN floorsensor left  */
#define AN_LINE_L     2        /*!< ADC-PIN liniesensor left  */
#define AN_LINE_R     3        /*!< ADC-PIN liniesensor right */
#define AN_VBAT       7        /*!< ADC-PIN battery voltage   */


/*! RS-Leitung des Displays
 *  legt fest, ob die Daten an das Display in den Textpuffer (RS=1) kommen
 *  oder als Steuercode interpretiert werden (RS=0)
 */
#define IO_DISPLAY_RS       IO_DISPLAY_RS
#define IO_DISPLAY_RS_PORT  PORTG
#define IO_DISPLAY_RS_BIT   3
#define IO_DISPLAY_RS_DDR   DDRG

/*! Read/Write Signal des Displays
 *  legt fest, ob zum Display geschrieben wird (RW=0)
 *  oder davon gelesen wird (RW=1)
 */
#define IO_DISPLAY_RW       IO_DISPLAY_RW
#define IO_DISPLAY_RW_PORT  PORTG
#define IO_DISPLAY_RW_BIT   4
#define IO_DISPLAY_RW_DDR   DDRG

/*! Enable Leitung des Displays
 *  schaltet das Interface ein (E=1)
 *  Nur wenn Enable auf High-Pegel liegt, läßt sich das Display ansprechen
 */
#define IO_DISPLAY_EN       IO_DISPLAY_EN
#define IO_DISPLAY_EN_PORT  PORTG
#define IO_DISPLAY_EN_BIT   2
#define IO_DISPLAY_EN_DDR   DDRG

#define IO_DISPLAY_CS1       IO_DISPLAY_CS1
#define IO_DISPLAY_CS1_PORT  PORTB
#define IO_DISPLAY_CS1_BIT   4
#define IO_DISPLAY_CS1_DDR   DDRB

#define IO_DISPLAY_CS2       IO_DISPLAY_CS2
#define IO_DISPLAY_CS2_PORT  PORTB
#define IO_DISPLAY_CS2_BIT   7
#define IO_DISPLAY_CS2_DDR   DDRB

#define IO_DISPLAY_RST       IO_DISPLAY_RST
#define IO_DISPLAY_RST_PORT  PORTB
#define IO_DISPLAY_RST_BIT   0
#define IO_DISPLAY_RST_DDR   DDRB

/*! Display Port */
#define IO_DISPLAY_PORT  PORTA
#define IO_DISPLAY_PIN   PINA
#define IO_DISPLAY_DDR   DDRA


/*! ISP Features */
#define IO_RESET_A_PORT  PORTD
#define IO_RESET_A_BIT   4
#define IO_RESET_A_DDR   DDRD

#define IO_RESET_B_PORT  PORTD
#define IO_RESET_B_BIT   5
#define IO_RESET_B_DDR   DDRD

#define IO_RESET_5_PORT  PORTD
#define IO_RESET_5_BIT   6
#define IO_RESET_5_DDR   DDRD

#define IO_RESET_3_PORT  PORTD
#define IO_RESET_3_BIT   7
#define IO_RESET_3_DDR   DDRD


#define IO_ISP_SCK_PORT  PORTB
#define IO_ISP_SCK_BIT   1
#define IO_ISP_SCK_DDR   DDRB

#define IO_ISP_MOSI_PORT  PORTB
#define IO_ISP_MOSI_BIT   2
#define IO_ISP_MOSI_DDR   DDRB

#define IO_ISP_MISO_PORT  PORTB
#define IO_ISP_MISO_PIN   PINB
#define IO_ISP_MISO_BIT   3
#define IO_ISP_MISO_DDR   DDRB


/* I2C Bus definitions */
#define I2C_BUF_SIZE 16
//#define I2C_TWBR_INIT 144   /* 50 kHz */
//#define I2C_TWBR_INIT 255   /* 25 kHz (?) */
//#define I2C_TWBR_INIT 12   /* 400 kHz */
#define I2C_TWBR_INIT 72   /* 100 kHz */


#define DISTCO_I2C_ID 42
#define MOTCO_I2C_ID 44

/* Hilfs-Macro */
#define _PPCAT(a,b) a##b

/* Macro-Funktionen für IO-Bits */
#define set_output_bit(NAME) _PPCAT(NAME,_PORT)|=_BV(_PPCAT(NAME,_BIT))
#define clear_output_bit(NAME) _PPCAT(NAME,_PORT)&=~(unsigned char)_BV(_PPCAT(NAME,_BIT))
#define set_output_bitval(NAME, VAL) _PPCAT(NAME,_PORT)=(_PPCAT(NAME,_PORT)&~(unsigned char)_BV(_PPCAT(NAME,_BIT)))|((VAL)?_BV(_PPCAT(NAME,_BIT)):0)

#define activate_output_bit(NAME) _PPCAT(NAME,_DDR)|=_BV(_PPCAT(NAME,_BIT))
#define deactivate_output_bit(NAME) _PPCAT(NAME,_DDR)&=~(unsigned char)_BV(_PPCAT(NAME,_BIT))

#define get_input_bit(NAME) (_PPCAT(NAME,_PIN)&_BV(_PPCAT(NAME,_BIT)))
#define get_output_bit(NAME) (_PPCAT(NAME,_PORT)&_BV(_PPCAT(NAME,_BIT)))

