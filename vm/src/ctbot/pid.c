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

/*! @file       pid.c
 * @brief       Modul zur PID-Regelung der Motoren
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2007-01-05
*/

#include "ct-Bot.h"
#include "ctbot/pid.h"
#include "ctbot/pwm.h"

#define PID_FREQ 50        // 50 mal pro Sekunde
#define PID_FACTOR 20      // alle Werte sind intern *20
#define PID_MAX_OUT 500    // maximaler Ausgabewert
#define PID_HIST_SIZE 10   // Filterung für Geschwindigkeit

#define PID_PROP_ 2.0      // proportionaler Anteil
#define PID_INTG_ 0.1      // integrativer Anteil
#define PID_DIFF_ 0        // -> momentan nur PI-Regler

#define PID_DIR_INTG 0.1   // integrativer Richtungsanteil


#define PID_PROP ((int)(PID_PROP_*PID_FACTOR))
#define PID_INTG ((int)(PID_INTG_*PID_FACTOR))
#define PID_DIFF ((int)(PID_DIFF_*PID_FACTOR))
#define PID_INT_MAX ((int)(PID_MAX_OUT*PID_FACTOR/(PID_INTG?PID_INTG:1)))
#define PID_INT_DIR_MAX ((int)(PID_MAX_OUT*PID_FACTOR/(PID_DIR_INTG?PID_DIR_INTG:1)))


int16_t pid_left_speed;
int16_t pid_right_speed;

int16_t pid_left_value;
int16_t pid_right_value;

int8_t pid_left_cnt;
int8_t pid_right_cnt;

uint8_t pid_mode;

static int16_t pid_left_sum;
static int16_t pid_right_sum;
static int16_t pid_dir_sum;

static int16_t pid_left_last;
static int16_t pid_right_last;


static int8_t pid_left_hist[PID_HIST_SIZE];
static int8_t pid_right_hist[PID_HIST_SIZE];

static int8_t pid_left_hist_pos;
static int8_t pid_right_hist_pos;

void pid_reset(void)
{
  pid_left_sum=0;
  pid_right_sum=0;
  pid_dir_sum=0;
}

void pid_left_timer_int(void)
{
  int16_t speed = pid_left_cnt;
  for (int i=0; i<PID_HIST_SIZE; ++i)
    speed+=pid_left_hist[i];
  speed *= PID_FREQ;
  speed /= PID_HIST_SIZE+1;
  pid_left_speed=speed;

  if (pid_mode & PID_LEFT_ENABLED)
  {
    // P-Teil
    int16_t delta = pid_left_value - speed; // soll-ist

    // D-Teil
    int16_t diff = delta-pid_left_last;
    pid_left_last = delta;

    // I-Teil
    pid_left_sum += delta;
    if (pid_left_sum > PID_INT_MAX) pid_left_sum=PID_INT_MAX;
    if (pid_left_sum < -PID_INT_MAX) pid_left_sum=-PID_INT_MAX;

    // Richtungs I-Teil
    pid_dir_sum += delta;
    if (pid_dir_sum > PID_INT_DIR_MAX) pid_dir_sum=PID_INT_DIR_MAX;
    if (pid_dir_sum < -PID_INT_DIR_MAX) pid_dir_sum=-PID_INT_DIR_MAX;

    // Ausgabewert berechnen
    int16_t output = PID_PROP * delta
                   + PID_INTG * pid_left_sum
                   + PID_DIFF * diff
                   + PID_DIR_INTG * pid_dir_sum;
    if (output<-PID_MAX_OUT*PID_FACTOR) output = -PID_MAX_OUT*PID_FACTOR;
    if (output>PID_MAX_OUT*PID_FACTOR) output = PID_MAX_OUT*PID_FACTOR;

    pwm_motor_l = output / PID_FACTOR;
  }

  if (pid_left_hist_pos==PID_HIST_SIZE)
    pid_left_hist_pos=0;
  pid_left_hist[pid_left_hist_pos++]=pid_left_cnt;
  pid_left_cnt=0;
}

void pid_right_timer_int(void)
{
  int16_t speed = pid_right_cnt;
  for (int i=0; i<PID_HIST_SIZE; ++i)
    speed+=pid_right_hist[i];
  speed *= PID_FREQ;
  speed /= PID_HIST_SIZE+1;
  pid_right_speed=speed;

  if (pid_mode & PID_RIGHT_ENABLED)
  {
    // P-Teil
    int16_t delta = pid_right_value - speed; // soll-ist

    // D-Teil
    int16_t diff = delta-pid_right_last;
    pid_right_last = delta;

    // I-Teil
    pid_right_sum += delta;
    if (pid_right_sum > PID_INT_MAX) pid_right_sum=PID_INT_MAX;
    if (pid_right_sum < -PID_INT_MAX) pid_right_sum=-PID_INT_MAX;

    // Richtungs I-Teil
    pid_dir_sum -= delta;
    if (pid_dir_sum > PID_INT_DIR_MAX) pid_dir_sum=PID_INT_DIR_MAX;
    if (pid_dir_sum < -PID_INT_DIR_MAX) pid_dir_sum=-PID_INT_DIR_MAX;

    // Ausgabewert berechnen
    int16_t output = PID_PROP * delta
                   + PID_INTG * pid_right_sum
                   + PID_DIFF * diff
                   - PID_DIR_INTG * pid_dir_sum;
    if (output<-PID_MAX_OUT*PID_FACTOR) output = -PID_MAX_OUT*PID_FACTOR;
    if (output>PID_MAX_OUT*PID_FACTOR) output = PID_MAX_OUT*PID_FACTOR;

    pwm_motor_r = output / PID_FACTOR;
  }

  if (pid_right_hist_pos==PID_HIST_SIZE)
    pid_right_hist_pos=0;
  pid_right_hist[pid_right_hist_pos++]=pid_right_cnt;
  pid_right_cnt=0;
}

