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

/*!
 *  @file    ir-rc5.c
 *  @brief   Routinen für die Dekodierung von RC5-Fernbedienungs-Codes
 *  @author  Benjamin Benz (bbe@heise.de)
 *  @author  modified by Nils Springob (nils@nicai-systems.de)
 *  @date    2007-01-26
 */

// Infos ueber RC6: http://www.xs4all.nl/~sbp/knowledge/ir/rc6.htm
// http://www.xs4all.nl/~sbp/knowledge/ir/ir.htm

// ========================================================================
// RC5 Infrarot-Empfänger
// ========================================================================
#include "ctbot/ct-Bot.h"
#ifdef MCU
#ifdef IR_AVAILABLE

#include <avr/io.h>
#include "ctbot/ir_rc5.h"
#include "ctbot/iodefs.h"

// -----------------------------------------------------------------------------
// Timer
// -----------------------------------------------------------------------------
#define IR_TIMER_CLOCK     5619
#define IR_SIGNAL_OC       SIG_OUTPUT_COMPARE2
#define IR_TIMER_CNT       TCNT2
#define IR_TIMER_CCR       TCCR2
#define IR_TIMER_CCR_INIT  _BV(WGM21) | _BV(CS22)
#define IR_TIMER_OCR       OCR2
#define IR_TIMER_OCR_INIT  ((F_CPU/64/IR_TIMER_CLOCK) - 1 )
#define IR_TIMER_IMSK      _BV(OCIE2)


// -----------------------------------------------------------------------------
// Timing
// -----------------------------------------------------------------------------
#define IR_SAMPLES_PER_BIT       10  /*!< 10 Samples per Bit */
#define IR_SAMPLES_PER_BIT_EARLY  8  /*!< Flanke fr�hestens nach 8 Samples */
#define IR_SAMPLES_PER_BIT_LATE  12  /*!< Flanke sp�testens nach 12 Samples */
#define IR_SAMPLES_PER_BIT_MIN    3  /*!< Flanke vor 3 Samples -> paket verwerfen */
/*!
 * Das Startbit ist erst nach 250 Samples ohne Pegeländerung gültig -- eigentlich
 * müsste man rund 500 Samples abwarten (50 x Bitzeit), doch weil der
 * Samplezähler ein Byte ist, beschränken wir uns hier auf ein Minimum
 * von 250 Samples
 */
#define IR_PAUSE_SAMPLES        250


volatile static uint8_t ir_lastsample; /*!< zuletzt gelesenes Sample */
volatile static uint8_t ir_bittimer;   /*!< zählt die Aufrufe von ir_isr() */
volatile static uint16_t ir_data_tmp;  /*!< RC5-Bitstream */
volatile static uint8_t  ir_bitcount;  /*!< anzahl gelesener bits */

volatile uint16_t ir_data;	/*!< letztes komplett gelesenes RC5-Paket */

/*!
 * Interrupt Serviceroutine
 * wird ca alle 177.8us aufgerufen
 */
SIGNAL (IR_SIGNAL_OC) {
	// sample lesen
	uint8_t sample = 1;

        if (get_input_bit(IO_IR) != 0) {
		sample = 0;
	}
	
	// bittimer erhoehen (bleibt bei 255 stehen)
	if (ir_bittimer<255) {
		ir_bittimer++;
	}

	// flankenerkennung
	if ( ir_lastsample != sample) {
		if (ir_bittimer<=IR_SAMPLES_PER_BIT_MIN) {
			// flanke kommt zu frueh: paket verwerfen
			ir_bitcount=0;
		} else {
			// Startbit
			if (ir_bitcount==0) {
				if ( (sample==1) && (ir_bittimer>IR_PAUSE_SAMPLES) ) {
					// Startbit speichern
					ir_data_tmp = 1;
					ir_bitcount++;
				} else {
					// error
					ir_data_tmp = 0;
				}
				
				// bittimer-reset
				ir_bittimer = 0;
				
			// Bits 2..14: nur Flanken innerhalb des Bits beruecksichtigen
			} else {
				if (ir_bittimer >= IR_SAMPLES_PER_BIT_EARLY) {
					if(ir_bittimer<=IR_SAMPLES_PER_BIT_LATE){
						// Bit speichern
						ir_data_tmp = (ir_data_tmp<<1) | sample;
						ir_bitcount++;
					} else {
						// zu spät: Paket verwerfen
						ir_bitcount = 0;
					}
					
					// bittimer-reset
					ir_bittimer = 0;
				}
			}
		}
		
	} else {
		// keine Flanke innerhalb bitzeit?
		if (ir_bittimer > IR_SAMPLES_PER_BIT_LATE) {
			// 14 bits gelesen?
			if (ir_bitcount==14) {
				ir_data = ir_data_tmp;
			}
			// Paket verwerfen
			ir_bitcount = 0;
		}
	}
	
	// Sample im Samplepuffer ablegen
	ir_lastsample = sample;
	

}


/*!
 * IR-Daten lesen
 * @return wert von ir_data, löscht anschliessend ir_data
 */
uint16_t ir_read(void) {
  uint16_t retvalue = ir_data;
  ir_data = 0;
  return retvalue;
}

/*!
 * Init IR-System
 */
void ir_init(void) {
  deactivate_output_bit(IO_IR); // Pin auf Input
  set_output_bit(IO_IR);        // Pullup an
  IR_TIMER_CNT = 0;
  IR_TIMER_CCR = IR_TIMER_CCR_INIT;
  IR_TIMER_OCR = IR_TIMER_OCR_INIT;
  TIMSK  |= IR_TIMER_IMSK;
}
#endif
#endif
