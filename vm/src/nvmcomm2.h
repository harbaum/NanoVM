//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2006 by Till Harbaum <Till@Harbaum.org>,
//                        Oliver Schulz <whisp@users.sf.net>
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
// NVM-Comm is the NanoVM upload and communication protocol.
// NVM-Comm v2.0 is more versatile and robust than v1.0, but also more
// complex.
// Since NVM-Comm v2.0 is still under development, NanoVM is currently
// compiled for NVM-Comm v1.0 by default.
//


#ifndef NVMCOMM2_H
#define NVMCOMM2_H


#ifdef AVR
#include <util/crc16.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#endif

#ifdef UNIX
#include <stdlib.h>
#endif

#include "types.h"
#include "nvmcomm1.h"
#include "loader.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef NVMCOMM2



#define NVM_RUNLVL_HALT    0x00
#define NVM_RUNLVL_CONF    0x01
#define NVM_RUNLVL_VM      0x03
#define NVM_RUNLVL_RESET   0x06
#define NVM_RUNLVL_BOOT    0x07


extern u08_t g_nvm_runlevel;   // NanoVM system runlevel


void nvm_showstatus_booting();

void nvm_showstatus_nvmcomm();

void nvm_showstatus_running();

void nvm_showstatus_halted();


void nvm_halt();

void nvm_reset();



// CRC-16-CCITT, msb-first, Polynomial: x^16 + x^12 + x^5 + 1
static inline uint16_t crc16_ccitt_msbf_update(uint8_t data, uint16_t crc) {
#ifdef AVR
	crc = _crc_xmodem_update(crc, data);
#else
	crc  = ((crc >> 8) & (0x00ff)) | (crc << 8);
	crc ^= data;
	crc ^= (crc & 0xff) >> 4;
	crc ^= (crc << 8) << 4;
	crc ^= ((crc & 0xff) << 4) << 1;
#endif
	return crc;
}


// CRC-16-CCITT, lsb-first, Polynomial: x^16 + x^12 + x^5 + 1
static inline uint16_t crc16_ccitt_lsbf_update(uint8_t data, uint16_t crc) {
#ifdef AVR
	crc = _crc_ccitt_update(crc, data);
#else
	data ^= (crc & 0xFF);
	data ^= data << 4;

	crc  = ((uint16_t)data << 8) | (crc >> 8);
	crc ^= (uint8_t)(data >> 4);
	crc ^= (uint16_t)(data << 3);
#endif
	return crc;
}



#define NVC2_MAX_MSG_LEN 0x7F    // Maximum message length

#define NVC2_BUFFER_SIZE 16      // NVC2 I/O data buffer size

#define NVC2_MAX_FID 0           // maximum supported file id
#define NVC2_FILE_FIRMWARE 0x00  // firmware file id

#define NVC2_CMD_FOPEN   0x70
#define NVC2_CMD_FCLOSE  0x71
#define NVC2_CMD_RWFILE  0x72
#define NVC2_CMD_RUNLVL  0x7E

#define NVC2_STATUS_ENABLED   (1<<7)  // 1/0: NVM-Comm2 enabled/disabled
#define NVC2_STATUS_AVAIL     (1<<6)  // 1: Recieved query is available
#define NVC2_STATUS_TRACKING  (1<<5)  // 1: Tracking a message
#define NVC2_STATUS_IGNORE    (1<<4)  // 1: Ignore this message
#define NVC2_QUERY_INVALID    (1<<3)  // 1: Invalid Message (CRC Error or incomplete)
#define NVC2_QUERY_REPEATED   (1<<2)  // 1: Received a repeated message
#define NVC2_QUERY_READ       (1<<1)  // 1/0 : Recieved message is a read/write query
#define NVC2_QUERY_COMMAND    (1<<0)  // 1: Recieved query contains a command

#define NVC2_TIMEOUT_CHARS 4  // Input timout in units of byte transmission time

#ifdef UART_BITRATE
#define NVC2_RECV_UART_TIMEOUT NVC2_TIMEOUT_CHARS*(10000/UART_BITRATE) + 1 // UART timeout during message reception / ms
#else
#define NVC2_RECV_UART_TIMEOUT 1 // UART timeout during message reception / ms
#endif

extern u08_t g_nvc2_address;       // System address
extern u08_t g_nvc2_msg_status;    // NVM-Comm2 tracked message status byte
extern u08_t g_nvc2_query_status;  // NVM-Comm2 query status byte
extern u08_t g_nvc2_track;         // NVM-Comm2 tracking position
extern u08_t g_nvc2_le;            // NVM-Comm2 message length
extern u08_t g_nvc2_msgid;         // message id
extern u08_t g_nvc2_lastid;        // last message id
extern u16_t g_nvc2_crc;           // message crc
extern u08_t g_nvc2_command;       // Used only if (g_nvc2_msg_status & NVC2_QUERY_COMMAND) > 0.

// For write queries, g_nvc2_data contains the write data,
// g_nvc2_dsize is set to the data length.
// For read queries, g_nvc2_data[0] is set to RSP, g_nvc2_dsize is not set.
// On reception of invalid or repeated messages, g_nvc2_data is not modified.
extern u08_t g_nvc2_data[NVC2_BUFFER_SIZE];
extern size8_t g_nvc2_dsize;

extern s08_t g_nvc2_file_open;     // open file id (id 0 = firmware);
extern u16_t g_nvc2_file_pos;      // read/write position in open file

extern bool g_nvc2_query_success;  // success of last query
extern u08_t g_nvc2_query_rsize;   // last response length


// Check if NVM-Comm2 enabled
static inline bool nvc2_enabled()
	{ return (g_nvc2_msg_status & NVC2_STATUS_ENABLED) > 0; }

// Enable NVM-Comm2
static inline void nvc2_enable()
	{ if (!nvc2_enabled()) g_nvc2_msg_status = NVC2_STATUS_ENABLED; }

// Disable NVM-Comm2
static inline void nvc2_disable()
	{ if (!nvc2_enabled()) g_nvc2_msg_status = 0x00; }


// Check if a new query is available
static inline bool nvc2_query_available()
	{ return (g_nvc2_msg_status & NVC2_STATUS_AVAIL) > 0; }

// Free things for the next query.
static inline void nvc2_query_finished() { g_nvc2_msg_status &= (0xFF ^ NVC2_STATUS_AVAIL); }


// Check if query is invalid	
static inline bool nvc2_query_invalid()
	{ return (g_nvc2_query_status & NVC2_QUERY_INVALID) > 0; }

// Check if query is repeated
static inline bool nvc2_query_repeated()
	{ return (g_nvc2_query_status & NVC2_QUERY_REPEATED) > 0; }

// Check if query is read or write
static inline bool nvc2_query_read()
	{ return (g_nvc2_query_status & NVC2_QUERY_READ) > 0; }

// Respone length, use only for read queries.
static inline size8_t nvc2_query_rsize() { return g_nvc2_data[0]; }

// Data length, use only for write queries.
static inline size8_t nvc2_query_dsize() { return g_nvc2_dsize; }

// Check if query includes a command (else it's an empty query).
static inline bool nvc2_query_hascmd()
	{ return (g_nvc2_query_status & NVC2_QUERY_COMMAND) > 0; }

// Data length, use only if nvc2_query_hascmd()==true.
static inline size8_t nvc2_query_command() { return g_nvc2_command; }



bool nvc2_proc_input(u08_t value);

//void nvc2_send_response();

//void nvc2_process_query();

void nvc2_check_input();


#endif // NVMCOMM2

#ifdef __cplusplus
}
#endif

#endif // NVMCOMM2_H
