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

/*! @file   sensor-low.c
 *  @brief  Low-Level Routinen für die Sensor Steuerung des c't-Bots
 *  @author Benjamin Benz (bbe@heise.de)
 *  @author modified by Nils Springob (nils@nicai-systems.de)
 *  @date   2007-01-08
*/

#include <avr/io.h>
#include "ctbot/iodefs.h"
#include "ctbot/wheelencoder.h"
#include "ctbot/pwm.h"
#include "ctbot/pid.h"

#define ENC_ENTPRELL 2 /*!< Nur wenn der Encoder ein paar mal den gleichen wert gibt uebernehmen */

#define FORWARDS 1
#define BACKWARDS 0

volatile uint8_t enc_l;  /*!< Puffer fuer die letzten Encoder-Staende */
volatile uint8_t enc_r;  /*!< Puffer fuer die letzten Encoder-Staende */

volatile uint8_t enc_l_cnt; /*!< Entprell-Counter fuer L-Encoder */
volatile uint8_t enc_r_cnt; /*!< Entprell-Counter fuer R-Encoder */

uint16_t wheelencoder_l;
uint16_t wheelencoder_r;

uint8_t inc_l;
uint8_t inc_r;

/*!
 * Initialisiere alle Sensoren
 */
void wheelencoder_init(void){
  deactivate_output_bit(IO_SENS_ENCL);
  deactivate_output_bit(IO_SENS_ENCR);
  wheelencoder_l = 0;
  wheelencoder_r = 0;
  inc_l = FORWARDS;
  inc_r = FORWARDS;
  pid_left_cnt = 0;
  pid_right_cnt = 0;
  enc_l_cnt = ENC_ENTPRELL+1;
  enc_r_cnt = ENC_ENTPRELL+1;
}

/*!
 * Kuemmert sich um die Radencoder
 * Das muss schneller gehen als die anderen Sensoren,
 * daher Update per Timer-Interrupt und nicht per Polling
 */
void wheelencoder_isr(void){
  // --------------------- links ----------------------------
  //Rad-Encoder auswerten
  uint8_t val = get_input_bit(IO_SENS_ENCL);
  if (val != enc_l){  // uns interesieren nur Veraenderungen
    enc_l=val;        // neuen wert sichern
    enc_l_cnt=0;      // Counter zuruecksetzen
  } else {            // Zaehlen, wie lange Pegel bleibt
    if (enc_l_cnt <= ENC_ENTPRELL) // Nur bis zur Entprell-Marke
      enc_l_cnt++;
  }

  if (enc_l_cnt == ENC_ENTPRELL){// wenn lange genug konst
    if (pwm_motor_l>0)  // Drehrichtung beachten
      inc_l=FORWARDS;
    else if (pwm_motor_l<0)
      inc_l=BACKWARDS;
    if (inc_l!=BACKWARDS){
      wheelencoder_l++;
      pid_left_cnt++;
    } else {
      wheelencoder_l--;
      pid_left_cnt--;
    }
  }

  // --------------------- rechts ----------------------------
  val = get_input_bit(IO_SENS_ENCR);
  if (val != enc_r){  // uns interesieren nur Veraenderungen
    enc_r=val;        // neuen wert sichern
    enc_r_cnt=0;
  } else{             // Zaehlen, wie lange Pegel bleibt
    if (enc_r_cnt <= ENC_ENTPRELL) // Nur bis zur Entprell-Marke
      enc_r_cnt++;
  }

  if (enc_r_cnt == ENC_ENTPRELL){ // wenn lange genug konst
    if (pwm_motor_r>0)  // Drehrichtung beachten
      inc_r=FORWARDS;
    else if (pwm_motor_r<0)
      inc_r=BACKWARDS;
    if (inc_r!=BACKWARDS){
      wheelencoder_r++;
      pid_right_cnt++;
    } else {
      wheelencoder_r--;
      pid_right_cnt--;
    }
    
  }
}
