//
// config.h
//

#ifndef CONFIG_H
#define CONFIG_H

#define CODESIZE 8192
#define HEAPSIZE 1024

// vm setup
#define NVM_USE_STACK_CHECK      // enable check if method returns empty stack
#define NVM_USE_ARRAY            // enable arrays
#define NVM_USE_SWITCH           // support switch instructions
#define NVM_USE_INHERITANCE      // support for inheritance
#define NVM_USE_HEAP_IDMAP       // enable heap id map
#define NVM_USE_DISK_FILE        // enable disk file loading

// native setup
#define NVM_USE_STDIO            // enable native stdio support

#endif // CONFIG_H
