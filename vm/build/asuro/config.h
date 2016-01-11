//
// config.h
//
// NanoVM configuration file for the Asuro
//

#ifndef CONFIG_H
#define CONFIG_H

// cpu related setup
#define ATMEGA8
#define ASURO                  // compiles some special code e.g. in uart.c
#define CLOCK 8000000

// uart setup
#define UART_BITRATE 2400
#define UART_BUFFER_BITS 5     // 32 bytes buffer (min. req for loader)

#define CODESIZE 512
#define HEAPSIZE 768

// avr specific native init routines
#define NATIVE_INIT  native_init()

// vm setup
#undef NVM_USE_STACK_CHECK      // enable check if method returns empty stack
#define NVM_USE_ARRAY            // enable arrays
#define NVM_USE_SWITCH           // support switch instruction
#define NVM_USE_INHERITANCE      // support for inheritance
#define NVM_USE_COMM             // enable firmware upload
#define NVM_USE_UTILS            // enable inline utils
#define NVM_USE_EEPROM           // nvm file resides in EEPROM (or flash)
#define NVM_USE_MEMCPY_UP        // enable custom memcpy for heap compacting
#define NVM_USE_DEFAULT_FILE     // enable pre-installed default file

// native setup
#define NVM_USE_STDIO            // enable native stdio support

// marker used to indicate, that this item is stored in eeprom
#define NVMFILE_FLAG     0x8000

#endif // CONFIG_H
