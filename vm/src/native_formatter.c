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

#include <string.h>

#include "types.h"
#include "debug.h"
#include "config.h"
#include "error.h"

#ifdef NVM_USE_FORMATTER

#include "stack.h"
#include "native.h"
#include "native_formatter.h"
#include "nvmstring.h"

#include <math.h>
#include <stdio.h>

#define NATIVE_METHOD_formatI 1
#define NATIVE_METHOD_formatZ 2
#define NATIVE_METHOD_formatF 3

typedef struct 
{
  u08_t flags;
  u08_t width;
  u08_t prec;
  u08_t conv;
  u08_t pre_len;
  u08_t post_len;
  char * post;
} formatDescr;

// extracts all digits and fill remaining digits with '0'
void inttostr(char * begin, char * end, u32_t val, u08_t base, char a)
{
  while (begin!=end--){
    char c = val%base;
    c += (c>9)?(a-10):'0';
    *end=c;
    val/=base;
  }
}



int format_int(char * res, formatDescr * fmtdscr, int val)
{
  int n=0;
  if (val<0){
    *res++='-', n++, val=-val;
  } else if (fmtdscr->flags&0x04) {
    *res++='+', n++;
  } else if (fmtdscr->flags&0x08) {
    *res++=' ', n++;
  }
  
  char a = 'a';
  char c = fmtdscr->conv;
  u08_t base = 10;
  if (c=='d'){
  }else if (c=='o'){
    base=8;
  }else if (c=='x'){
    base=16;
  }else if (c=='X'){
    base=16;
    a='A';
  }

  inttostr(res, res+10, val, base, a);
  u08_t prec = 10;
  u08_t i = 0;
  while (prec--){
    if (res[i++]!='0')
      break;
  }
  prec++;
  if (fmtdscr->flags&0x10) {
    if (fmtdscr->width-n > prec)
      prec = fmtdscr->width-n;
  }
  if (prec==0)
    prec=1;
  u08_t offset = 10-prec;
  while(prec--){
    res[0]=res[offset];
    res++, n++;
  }
  return n;
}

int format_bool(char * res, formatDescr * fmtdscr, int val)
{
  if (fmtdscr->conv=='b'){
    if (val){
      native_strcpy(res, "true");
      return 4;
    } else {
      native_strcpy(res, "false");
      return 5;
    }
  } else if (fmtdscr->conv=='B'){
    if (val){
      native_strcpy(res, "TRUE");
      return 4;
    } else {
      native_strcpy(res, "FALSE");
      return 5;
    }
  } else {
    return 0;
  }
}

#define FLOAT_DIGS 10

int format_float(char * res, formatDescr * fmtdscr, float val)
{
  int n=0;
  if (val<0){
    *res++='-', n++, val=-val;
  } else if (fmtdscr->flags&0x04) {
    *res++='+', n++;
  } else if (fmtdscr->flags&0x08) {
    *res++=' ', n++;
  }
  
  char a = 'a';
  char conv = fmtdscr->conv;
  u08_t base = 10;
  if (conv=='a'){
    base=16;
  }else if (conv=='A'){
    base=16;
    a='A';
  }

  s08_t max_prec = 8;
  s08_t prec = max_prec;

  float factA = 1.0;
  for (u08_t i=0; i<prec-1; ++i){
    factA*=(float)base;
  }
  float factB = base*factA;
  
  s08_t exp=0;

  if (val>0.0) {
    exp=prec-1;
    while(val>=factB)
      val/=base, exp+=1;
    while(val<factA)
      val*=base, exp-=1;
  }
  inttostr(res, res+prec, val, base, a);

  while (prec>0){
    if (res[prec-1]!='0')
      break;
    prec--;
  }
  //prec contains number of significant digits

  // wenn exp>=0 exp+1 = Ziffern vor dem Komma
  // wenn exp<0  |abs| = Nullen vor erster Ziffer
  if ((exp<-4) || (exp>prec)) {
    if (conv=='g') conv='e';
    if (conv=='G') conv='E';
  } else {
    if (conv=='g') conv='f';
    if (conv=='G') conv='f';
  }
  
  s08_t pre=1;
  if (conv=='f'){
    if (exp>=0) {
      pre += exp;
    } else {
      pre=0;
    }
  }
  if (prec>pre)
    prec -= pre;
  else
    prec=0;
  
  if (fmtdscr->prec != 255)
    prec = fmtdscr->prec;
  DEBUGF("formatting float prec=%i pre=%i exp=%i\n", (int)prec, (int)pre, (int)exp);

  if (fmtdscr->flags&0x02){
    if (prec==0)
      prec=1;
  }

  u08_t offs = 1;
  if (pre==0){
    offs = -exp + 1;
    prec += exp + 1;
    if (prec<0) {
      prec=0;
    }
  }
  for (s08_t i=pre+prec; i>=pre; i--){
    if (i>=max_prec)
      res[i+offs]='0';
    else
      res[i+offs]=res[i];
  }
  
  DEBUGF("formatting float prec=%i pre=%i exp=%i\n", (int)prec, (int)pre, (int)exp);
  if (pre==0) {
    *res++='0', n++;
    if (prec>0){
      *res++='.', n++;
      while(++exp<0)
        *res++='0', n++;
    } else {
      u08_t x = fmtdscr->prec;
      if ((x != 255)&&(x!=0)) {
        *res++='.', n++;
        while(x--)
          *res++='0', n++;
      }
    }
  } else if (prec>0) {
    res[pre]='.';
    res++, n++;
  } else {
  }

  res+=pre+prec, n+=pre+prec;

  if (conv!='f'){
    *res++=conv, n++;
    if (exp<0){
      *res++='-', n++;
      exp=-exp;
    } else {
      *res++='+', n++;
    }
    inttostr(res, res+2, exp, base, a);
    n+=2;
  }
  
  return n;
}

void make_format_descr(formatDescr * fmtdscr, char * fmt)
{
  u08_t mode=0;
  fmtdscr->flags = 0;
  fmtdscr->width = 0;
  fmtdscr->prec = 255;
  fmtdscr->conv = 0;
  fmtdscr->pre_len = 0;
  fmtdscr->post_len = 0;
  fmtdscr->post = 0;
  char c;
  while((c=native_getchar(fmt++))){
    switch (mode){
      case 0:
        if (c=='%') mode=1;
        else fmtdscr->pre_len++;
        break;

      case 1:
        if      (c=='-') fmtdscr->flags|=0x01;
        else if (c=='#') fmtdscr->flags|=0x02;
        else if (c=='+') fmtdscr->flags|=0x04;
        else if (c==' ') fmtdscr->flags|=0x08;
        else if (c=='0') fmtdscr->flags|=0x10;
        else if (c=='.') fmtdscr->prec=0, mode=3;
        else goto label2;
        break;

      label2:
      case 2:
        mode=2;
        if ((c>='0') && (c<='9')) {
          fmtdscr->width*=10;
          fmtdscr->width+=c-'0';
        } else if (c=='.'){
          fmtdscr->prec=0, mode=3;
        } else {
          goto label4;
        }
        break;

      case 3:
        if ((c>='0') && (c<='9')) {
          fmtdscr->prec*=10;
          fmtdscr->prec+=c-'0';
        } else {
          goto label4;
        }
        break;

      label4:
      case 4:
        mode=4;
        fmtdscr->conv=c;
        fmtdscr->post=fmt;
        mode=5;
        break;
        
      case 5:
        fmtdscr->post_len++;
    }
  }
}


void native_formatter_init(void) {
}

void native_formatter_invoke(u08_t mref) {
  char * fmt = stack_peek_addr(0);
  char res[50];
  
  formatDescr fmtdscr;
  make_format_descr(&fmtdscr, fmt);

  int len = 0;
  if(mref == NATIVE_METHOD_formatI) {
    nvm_int_t val = stack_peek_int(1);
    len = format_int(res, &fmtdscr, val);
  } else if(mref == NATIVE_METHOD_formatZ) {
    nvm_int_t val = stack_peek_int(1);
    len = format_bool(res, &fmtdscr, val);
  } else if(mref == NATIVE_METHOD_formatF) {
    nvm_float_t val = stack_peek_float(1);
    len = format_float(res, &fmtdscr, val);
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
    

  u08_t add = 0;
  if (fmtdscr.width>len)
    add = (fmtdscr.width-len);
    
  // allocate heap and realign strings (address may be changed by gc...)
  heap_id_t id = heap_alloc(FALSE, len + add + fmtdscr.pre_len + fmtdscr.post_len + 1);
  int memoffset = (char*)stack_peek_addr(0)-(char*)fmt;
  fmt+=memoffset;
  fmtdscr.post+=memoffset;
  char * dst = heap_get_addr(id);
  
  // build result string
  native_strncpy(dst, fmt, fmtdscr.pre_len);
  dst+=fmtdscr.pre_len;

  if (!(fmtdscr.flags&0x01)){
    while(add--)
      *dst++=' ';
  }

  native_strncpy(dst, res, len);
  dst+=len;
  
  if (fmtdscr.flags&0x01){
    while(add--)
      *dst++=' ';
  }
  native_strncpy(dst, fmtdscr.post, fmtdscr.post_len);
  dst+=fmtdscr.post_len;
  *dst=0;
  stack_pop();
  stack_pop();
  stack_push(NVM_TYPE_HEAP | id);
}

#endif //NVM_USE_MATH

