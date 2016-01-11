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

#include <avr/io.h>

/*! linker Motor Enable Signal */
#define IO_MOTOR_L_PORT  PORTD
#define IO_MOTOR_L_BIT   5
#define IO_MOTOR_L_DDR   DDRD

/*! rechter Motor Enable Signal */
#define IO_MOTOR_R_PORT  PORTD
#define IO_MOTOR_R_BIT   4
#define IO_MOTOR_R_DDR   DDRD

/*! Servo 1 */
#define IO_SERVO_1_PORT  PORTB
#define IO_SERVO_1_BIT   3
#define IO_SERVO_1_DDR   DDRB

/*! Servo 2 */
#define IO_SERVO_2_PORT  PORTD
#define IO_SERVO_2_BIT   7
#define IO_SERVO_2_DDR   DDRD

/*! linker Motor Drehrichtung */
#define IO_MOTOR_L_DIR_PORT  PORTC
#define IO_MOTOR_L_DIR_BIT   6
#define IO_MOTOR_L_DIR_DDR   DDRC

/*! rechter Motor Drehrichtung */
#define IO_MOTOR_R_DIR_PORT  PORTC
#define IO_MOTOR_R_DIR_BIT   7
#define IO_MOTOR_R_DIR_DDR   DDRC

/*! linker Rad Encoder */
#define IO_SENS_ENCL_PIN  PINB
#define IO_SENS_ENCL_BIT  4
#define IO_SENS_ENCL_DDR  DDRB

/*! rechter Rad Encoder */
#define IO_SENS_ENCR_PIN  PIND
#define IO_SENS_ENCR_BIT  3
#define IO_SENS_ENCR_DDR  DDRD

/*! IR Receiver */
#define IO_IR_PORT  PORTB
#define IO_IR_BIT   1
#define IO_IR_DDR   DDRB
#define IO_IR_PIN   PINB

/*! Maus Taktleitung */
#define IO_MAUS_SCK_PORT  PORTB
#define IO_MAUS_SCK_BIT   7
#define IO_MAUS_SCK_DDR   DDRB

/*! Maus Datenleitung */
#define IO_MAUS_SDA_PORT  PORTB
#define IO_MAUS_SDA_PIN   PINB
#define IO_MAUS_SDA_BIT   6
#define IO_MAUS_SDA_DDR   DDRB

/*! Fehlerüberwachung */
#define IO_SENS_ERROR_PIN  PINB
#define IO_SENS_ERROR_BIT  2
#define IO_SENS_ERROR_DDR  DDRB

/*! Lichtschranke Transportfachueberwachung */
#define IO_SENS_SCHRANKE_PIN  PINB
#define IO_SENS_SCHRANKE_BIT  0
#define IO_SENS_SCHRANKE_DDR  DDRB

/*! Klappensensor */
#define IO_SENS_DOOR_PIN  PIND
#define IO_SENS_DOOR_BIT  6
#define IO_SENS_DOOR_DDR  DDRD

/*! RS-Leitung des Displays
 *  legt fest, ob die Daten an das Display in den Textpuffer (RS=1) kommen
 *  oder als Steuercode interpretiert werden (RS=0)
 */
#define IO_DISPLAY_RS       IO_DISPLAY_RS
#define IO_DISPLAY_RS_PORT  PORTC
#define IO_DISPLAY_RS_BIT   0
#define IO_DISPLAY_RS_DDR   DDRC

/*! Read/Write Signal des Displays
 *  legt fest, ob zum Display geschrieben wird (RW=0)
 *  oder davon gelesen wird (RW=1)
 */
#define IO_DISPLAY_RW       IO_DISPLAY_RW
#define IO_DISPLAY_RW_PORT  PORTC
#define IO_DISPLAY_RW_BIT   1
#define IO_DISPLAY_RW_DDR   DDRC

/*! Enable Leitung des Displays
 *  schaltet das Interface ein (E=1)
 *  Nur wenn Enable auf High-Pegel liegt, läßt sich das Display ansprechen
 */
#define IO_DISPLAY_EN       IO_DISPLAY_EN
#define IO_DISPLAY_EN_PORT  PORTC
#define IO_DISPLAY_EN_BIT   2
#define IO_DISPLAY_EN_DDR   DDRC

/*! Display Port */
#define IO_DISPLAY_PORT  PORTC
#define IO_DISPLAY_DDR   DDRC

/*! Display Reday Flag (funktioniert nicht!) */
#define IO_DISPLAY_READY_PIN  ???? PINC ????
#define IO_DISPLAY_READY_DDR  DDRC
#define IO_DISPLAY_READY_BIT  5

/* Enable port */
#define ENA_ABSTAND   (1<<0)  /*!< Enable-Leitung Abstandssensoren */
#define ENA_RADLED    (1<<1)  /*!< Enable-Leitung Radencoder */
#define ENA_SCHRANKE  (1<<2)  /*!< Enable-Leitung Fachueberwachung */
#define ENA_KANTLED   (1<<3)  /*!< Enable-Leitung Angrundsensor */
#define ENA_KLAPPLED  (1<<4)  /*!< Enable-Leitung Schieberueberwachung */
#define ENA_MAUS      (1<<5)  /*!< Enable-Leitung Liniensensor auf Mausplatine */
#define ENA_ERW1      (1<<6)  /*!< Enable-Leitung Reserve 1 */
#define ENA_ERW2      (1<<7)  /*!< Enable-Leitung Reserve 2 */

/* Analog Kanäle */
#define SENS_ABST_L     0         /*!< ADC-PIN Abstandssensor Links */
#define SENS_ABST_R     1         /*!< ADC-PIN Abstandssensor Rechts */
#define SENS_M_L        2         /*!< ADC-PIN Liniensensor Links */
#define SENS_M_R        3         /*!< ADC-PIN Liniensensor Rechts */
#define SENS_LDR_L      4         /*!< ADC-PIN Lichtsensor Links */
#define SENS_LDR_R      5         /*!< ADC-PIN Lichtsensor Rechts */
#define SENS_KANTE_L    6         /*!< ADC-PIN Kantensensor Links */
#define SENS_KANTE_R    7         /*!< ADC-PIN Kantensensor Rechts */

/* Hilfs-Macro */
#define _PPCAT(a,b) a##b

/* Macro-Funktionen für IO-Bits */
#define set_output_bit(NAME) _PPCAT(NAME,_PORT)|=_BV(_PPCAT(NAME,_BIT))
#define clear_output_bit(NAME) _PPCAT(NAME,_PORT)&=~(unsigned char)_BV(_PPCAT(NAME,_BIT))
#define set_output_bitval(NAME, VAL) _PPCAT(NAME,_PORT)=(_PPCAT(NAME,_PORT)&~(unsigned char)_BV(_PPCAT(NAME,_BIT)))|((VAL)?_BV(_PPCAT(NAME,_BIT)):0)

#define activate_output_bit(NAME) _PPCAT(NAME,_DDR)|=_BV(_PPCAT(NAME,_BIT))
#define deactivate_output_bit(NAME) _PPCAT(NAME,_DDR)&=~(unsigned char)_BV(_PPCAT(NAME,_BIT))

#define get_input_bit(NAME) (_PPCAT(NAME,_PIN)&_BV(_PPCAT(NAME,_BIT)))

