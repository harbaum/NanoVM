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

/*! @file   mouse.c
 *  @brief  Routinen fuer die Ansteuerung eines opt. Maussensors
 *  @author Benjamin Benz (bbe@heise.de)
 *  @author modified by Nils Springob (nils@nicai-systems.de)
 *  @date   2007-01-11
*/

#include <avr/io.h>
#include "ct-Bot.h"
#include "mouse.h"
#include "delay.h"
#include "iodefs.h"

/*!
 * Uebertraegt ein Byte an den Sensor
 * @param data das Byte
 */
void maus_sens_writeByte(uint8_t data){
  uint8_t i;
  activate_output_bit(IO_MAUS_SDA);

  for (i=0; i<8; ++i){
    clear_output_bit(IO_MAUS_SCK);             // SCK=0 fallende Flanke
    set_output_bitval(IO_MAUS_SDA, data&0x80); // Daten rausschreiben
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    set_output_bit(IO_MAUS_SCK);               // SCK=1 Datenübernahme auf steigender Flanke
    data <<= 1;                                // naechstes Bit vorbereiten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
  }
}

/*!
 * Liest ein Byte vom Sensor
 * @return das Byte
 */
uint8_t maus_sens_readByte(void){
  uint8_t i;
  uint8_t data=0;

  deactivate_output_bit(IO_MAUS_SDA); // SDA auf Input

  for (i=0; i<8; ++i){
    clear_output_bit(IO_MAUS_SCK); // SCK =0 Sensor bereitet Daten auf fallender Flanke vor !
    data=data<<1;                  // Platz schaffen
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    set_output_bit(IO_MAUS_SCK);   // SCK =1 Daten lesen  auf steigender Flanke
    data |= get_input_bit(IO_MAUS_SDA)?1:0; //Daten lesen
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
    asm volatile("nop");                       // Kurz warten
  }
  return data;
}

/*!
 * Uebertraegt ein write-Kommando an den Sensor
 * @param adr Adresse
 * @param data Datum
 */
void maus_sens_write(int8_t adr, uint8_t data){
  maus_sens_writeByte(adr|=0x80);  //rl MSB muss 1 sein Datenblatt S.12 Write Operation
  maus_sens_writeByte(data);
  delay_us(100);
}

/*!
 * Schickt ein Lesekommando an den Sensor
 * und liest ein Byte zurueck
 * @param adr die Adresse
 * @return das Datum
 */
uint8_t maus_sens_read(uint8_t adr){
  maus_sens_writeByte(adr);
  delay_us(100);
  return maus_sens_readByte();
}

/*! 
 * Initialisiere Maussensor
 */ 
void maus_sens_init(void){
  delay(100);

  clear_output_bit(IO_MAUS_SCK);    // SCK auf 0
  activate_output_bit(IO_MAUS_SCK); // SCK auf Output

  delay_ms(10);
  maus_sens_write(MOUSE_CONFIG_REG,MOUSE_CFG_RESET);       //Reset sensor
  maus_sens_write(MOUSE_CONFIG_REG,MOUSE_CFG_FORCEAWAKE);  //Always on
}

/*! muessen wir nach dem ersten Pixel suchen?*/
static uint8_t firstRead;
/*!
 * Bereitet das auslesen eines ganzen Bildes vor
 */
void maus_image_prepare(void){
  maus_sens_write(MOUSE_CONFIG_REG,MOUSE_CFG_FORCEAWAKE); //Always on
  maus_sens_write(MOUSE_PIXEL_DATA_REG,0x00); // Frame grabben anstossen
  firstRead=1; //suche erstes Pixel
}

/*!
 * Liefert bei jedem Aufruf das naechste Pixel des Bildes
 * Insgesamt gibt es 324 Pixel
 * <pre>
 * 18 36 ... 324
 * .. .. ... ..
 *  2 20 ... ..
 *  1 19 ... 307
 * </pre>
 * Bevor diese Funktion aufgerufen wird, muss maus_image_prepare() aufgerufen werden!
 * @return Die Pixeldaten (Bit 0 bis Bit5), Pruefbit, ob Daten gueltig (Bit6), Markierung fuer den Anfang eines Frames (Bit7)
 */
uint8_t maus_image_read(void){
  int8_t pixel=maus_sens_read(MOUSE_PIXEL_DATA_REG);
  if ( firstRead ==1){
    while ( (pixel & 0x80) != 0x80){
      pixel=maus_sens_read(MOUSE_PIXEL_DATA_REG);
      //if ((pixel & 0x70) != 0x70)
      //  return 0;
    }
    firstRead=0;
  }
  return pixel;
}


