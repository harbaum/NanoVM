//
// config.h
//
// NanoVM configuration file for the avr atmega32 test board
//

#ifndef CONFIG_H
#define CONFIG_H

// cpu related setup
#define ATMEGA32
#define CLOCK 16000000UL

// uart setup
#define UART_BITRATE 9600
#define UART_BUFFER_BITS 5     // 32 bytes buffer (min. req for loader)

// use new comm protocol
// #define NVMCOMM2

#define CODESIZE 1024
#define HEAPSIZE (2048-256)    // NanoVM itself requires 256 Bytes RAM

// avr and lcd specific native init routines
#define NATIVE_INIT  { native_init(); native_lcd_init(); }

// vm setup
#undef NVM_USE_STACK_CHECK      // enable check if method returns empty stack
#define NVM_USE_ARRAY            // enable arrays
#define NVM_USE_SWITCH           // support switch instruction
#define NVM_USE_INHERITANCE      // support for inheritance
#define NVM_USE_UTILS            // enable inline utils
#define NVM_USE_EEPROM           // nvm file resides in EEPROM (or flash)
#define NVM_USE_MEMCPY_UP        // enable custom memcpy for heap compacting
#define NVM_USE_DEFAULT_FILE     // enable pre-installed default file

// native setup
#define NVM_USE_STDIO            // enable native stdio support

// native lcd interface
#define LCD
#define LCD_VIRTUAL_LINE_LENGTH  40   // byte offset between one line and the next
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR  DDRB
#define LCD_DATA_PORT PORTC
#define LCD_DATA_DDR  DDRC
#define LCD_DATA_PIN  PINC
#define LCD_CTRL_RS   1
#define LCD_CTRL_RW   2
#define LCD_CTRL_E    3

// marker used to indicate, that this item is stored in eeprom
#define NVMFILE_FLAG     0x8000

#define NVM_USE_FLOAT
#define NVM_USE_32BIT_WORD

#endif // CONFIG_H
