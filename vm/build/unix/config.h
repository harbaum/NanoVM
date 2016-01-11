//
// config.h
//

#ifndef CONFIG_H
#define CONFIG_H

#define CODESIZE 32768
#define HEAPSIZE 768

#define WDT_NO_STATISTICS  // all eeprom required for uvmfile

// define this if you don't want to use stdin/stdout, but
// a named pipe (e.g. to test the loader code):
// #define UART_PORT "/dev/ttyq0"

#define NVM_USE_STACK_CHECK      // enable check if method returns empty stack
#define NVM_USE_ARRAY            // enable arrays
#define NVM_USE_SWITCH           // support switch instructions
#define NVM_USE_INHERITANCE      // support for inheritance
#define NVM_USE_FLOAT            // floating point support
#define NVM_USE_32BIT_WORD       // 32 bit integer
#define NVM_USE_COMM             // enable firmware upload
#define NVM_USE_UTILS            // enable inline utils
#define NVM_USE_EEPROM           // nvm file resides in EEPROM (or flash)
#define NVM_USE_MEMCPY_UP        // enable custom memcpy for heap compacting
#define NVM_USE_DISK_FILE        // enable disk file loading
#define NVM_USE_DEFAULT_FILE     // enable pre-installed default file

// native setup
#define NVM_USE_MATH             // enable native math functions
#define NVM_USE_STDIO            // enable native stdio support
#define NVM_USE_FORMATTER        // enable native formatter class

// marker used to indicate, that this item is stored in eeprom
#define NVMFILE_FLAG       0x40000000

#endif // CONFIG_H

