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
//  main() for NanoVM runtime
//

#include <stdio.h>

#include "types.h"
#include "config.h"
#include "debug.h"

#include "loader.h"
#include "uart.h"
#include "nvmfile.h"
#include "vm.h"

// hooks for init routines

#include "native_impl.h"

/*
#ifdef ASURO
#include "native_asuro.h"
#endif

#ifdef NATIVE_AVR
#include "native_avr.h"
#endif

*/

#ifdef LCD
#include "native_lcd.h"
#endif

int main(int argc, char **argv) {

#if defined(UNIX) || defined(NVM_USE_COMM)
  uart_init();
#endif

  // call native initialization (e.g. hardware setup)
#ifdef NATIVE_INIT
  NATIVE_INIT;
#endif

#if defined(UNIX) || defined(__CC65__)
  // parse unix command line options and load 
  // nvm file if requested
  int i = 1, quiet = 0;

#ifdef DEBUG
  debug_enable(FALSE);
#endif

  // parse options
  while((i<argc)&&(argv[i][0] == '-')) {
#ifdef DEBUG
    if(argv[i][1] == 'd')
      debug_enable(TRUE);
#endif

    if(argv[i][1] == 'q')
      quiet = TRUE;

    i++;
  }

  if(!quiet)
    printf("NanoVM " VERSION " runtime (c) 2005-2007 by Till Harbaum <till@harbaum.org>\n");

#ifdef NVM_USE_DISK_FILE
  // load translated class file
  if((i<argc)&&(argv[i][0] != '-')) {
    nvmfile_load(argv[i], quiet);
  } else {
#ifdef NVM_USE_DEFAULT_FILE
    printf("running pre-installed default\n");
#else // NVM_USE_DEFAULT_FILE
    printf("Usage: NanoVM [-options] nvm-file\n"
           "Options:\n"
#ifdef DEBUG
           "  -d  Debug - Display debug information\n"
#endif
           "  -q  Quiet - Display no VM information\n");
    return -2;
#endif // NVM_USE_DEFAULT_FILE
  }
#endif // NVM_USE_DISK_FILE
#endif // UNIX || __CC65__

#ifdef NVM_USE_COMM
  // wait 1 sec for upload
  loader_receive();
#endif

  nvmfile_init();

  vm_init();

  nvmfile_call_main();

#ifdef UNIX
  // the following is only being done to detect memory leaks and
  // heap corruption and can thus be omitted on the real thing (tm)

  // give main args back to heap
  heap_unsteal(1*sizeof(nvm_stack_t));

  heap_garbage_collect();
  heap_show();
#endif // UNIX

  DEBUGF("main() returned\n");

#if !defined(UNIX) && !defined(__CC65__)
  // don't care for anything after this ...
  for(;;);  // reset wdt if in use
#endif

  return 0;
}
