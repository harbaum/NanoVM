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
//  native_lcd.c, native driver for Hitachi HD44780 based text LCDs
//

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef NVM_USE_MATH

#include "stack.h"
#include "native.h"
#include "native_math.h"

#include <math.h>
#include <stdlib.h>


#define NATIVE_METHOD_absF 1
#define NATIVE_METHOD_absI 2
#define NATIVE_METHOD_acos 3
#define NATIVE_METHOD_asin 4
#define NATIVE_METHOD_atan 5
#define NATIVE_METHOD_atan2 6
#define NATIVE_METHOD_ceil 7
#define NATIVE_METHOD_cos 8
#define NATIVE_METHOD_exp 9
#define NATIVE_METHOD_floor 10
#define NATIVE_METHOD_log 11
#define NATIVE_METHOD_maxF 12
#define NATIVE_METHOD_maxI 13
#define NATIVE_METHOD_minI 14
#define NATIVE_METHOD_minF 15
#define NATIVE_METHOD_pow 16
#define NATIVE_METHOD_random 17
#define NATIVE_METHOD_rint 18
#define NATIVE_METHOD_round 19
#define NATIVE_METHOD_sin 20
#define NATIVE_METHOD_sqrt 21
#define NATIVE_METHOD_tan 22
#define NATIVE_METHOD_toDegrees 23
#define NATIVE_METHOD_toRadians 24


void native_math_init(void) {
}

void native_math_invoke(u08_t mref) {
  if(mref == NATIVE_METHOD_absF) {
    stack_push(nvm_float2stack(fabs(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_absI) {
    stack_push(nvm_int2stack(abs(stack_pop_int())));
  
  } else if(mref == NATIVE_METHOD_acos) {
    stack_push(nvm_float2stack(acos(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_asin) {
    stack_push(nvm_float2stack(asin(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_atan) {
    stack_push(nvm_float2stack(atan(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_atan2) {
    nvm_float_t a = stack_pop_float();
    nvm_float_t b = stack_pop_float();
    stack_push(nvm_float2stack(atan2(a,b)));
  } else if(mref == NATIVE_METHOD_ceil) {
    stack_push(nvm_float2stack(ceil(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_cos) {
    stack_push(nvm_float2stack(cos(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_exp) {
    stack_push(nvm_float2stack(exp(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_floor) {
    stack_push(nvm_float2stack(floor(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_log) {
    stack_push(nvm_float2stack(log(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_maxF) {
    nvm_float_t a=stack_pop_float();
    nvm_float_t b=stack_pop_float();
    stack_push(nvm_float2stack(a>b?a:b));
  } else if(mref == NATIVE_METHOD_maxI) {
    nvm_int_t a=stack_pop_int();
    nvm_int_t b=stack_pop_int();
    stack_push(nvm_int2stack(a>b?a:b));
  } else if(mref == NATIVE_METHOD_minI) {
    nvm_int_t a=stack_pop_int();
    nvm_int_t b=stack_pop_int();
    stack_push(nvm_int2stack(a<b?a:b));
  } else if(mref == NATIVE_METHOD_minF) {
    nvm_float_t a=stack_pop_float();
    nvm_float_t b=stack_pop_float();
    stack_push(nvm_float2stack(a<b?a:b));
  } else if(mref == NATIVE_METHOD_pow) {
    nvm_float_t a = stack_pop_float();
    nvm_float_t b = stack_pop_float();
    stack_push(nvm_float2stack(pow(a,b)));
  } else if(mref == NATIVE_METHOD_random) {
    stack_push(nvm_float2stack((nvm_float_t)rand()/RAND_MAX));
  } else if(mref == NATIVE_METHOD_rint) {
    stack_push(nvm_float2stack((int)(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_round) {
    stack_push(nvm_int2stack((int)(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_sin) {
    stack_push(nvm_float2stack(sin(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_sqrt) {
    stack_push(nvm_float2stack(sqrt(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_tan) {
    stack_push(nvm_float2stack(tan(stack_pop_float())));
  } else if(mref == NATIVE_METHOD_toDegrees) {
    stack_push(nvm_float2stack(180/M_PI*stack_pop_float()));
  } else if(mref == NATIVE_METHOD_toRadians) {
    stack_push(nvm_float2stack(M_PI/180*stack_pop_float()));
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
}

#endif //NVM_USE_MATH

