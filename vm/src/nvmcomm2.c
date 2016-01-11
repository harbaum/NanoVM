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


#include "nvmcomm2.h"
#include "uart.h"
#include "delay.h"
#include "loader.h"
#include "nvmfile.h"


#ifdef NVMCOMM2


#ifdef AVR
#include <util/crc16.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#endif


u08_t g_nvm_runlevel = NVM_RUNLVL_BOOT;


void nvm_showstatus_booting() {
#ifdef ASURO
	// yellow status led
	PORTD |= _BV(2);
	PORTB |= _BV(0);
#endif
}


void nvm_showstatus_nvmcomm() {
#ifdef ASURO
	// red status led
	PORTD |= _BV(2);
	PORTB &= ~_BV(0);
#endif
}


void nvm_showstatus_running() {
#ifdef ASURO
	// green status led
	PORTD &= ~_BV(2);
	PORTB |= _BV(0);
#endif
}


void nvm_showstatus_halted() {
#ifdef ASURO
	// status led off
	PORTD &= ~_BV(2);
	PORTB &= ~_BV(0);
#endif
}


void nvm_halt() {
	nvm_showstatus_halted();
#ifdef AVR
	cli();
	wdt_disable();
while (true) uart_write_byte('H'); //!! DEBUG OUTPUT
	for(;;);
#else
	exit(0);
#endif
}


void nvm_reset() {
#ifdef AVR
	cli();
	wdt_enable(WDTO_15MS);
	for(;;);
#else
	exit(0);
#endif
}



u08_t g_nvc2_address = 0x10;
u08_t g_nvc2_msg_status = 0;
u08_t g_nvc2_query_status = 0;
u08_t g_nvc2_track  = 0;
u08_t g_nvc2_le     = 0;
u08_t g_nvc2_msgid  = 0;
u08_t g_nvc2_lastid = 0xFF;
u16_t g_nvc2_crc    = 0;
u08_t g_nvc2_command;

u08_t g_nvc2_data[NVC2_BUFFER_SIZE];
size8_t g_nvc2_dsize = 0;


s08_t g_nvc2_file_open = -1;
u16_t g_nvc2_file_pos = 0;

bool g_nvc2_query_success = false;
u08_t g_nvc2_query_rsize = 0;



static inline void nvc2_message_drop()
	{ g_nvc2_msg_status &= NVC2_STATUS_ENABLED | NVC2_STATUS_AVAIL; }


static inline bool nvc2_message_tracking()
	{ return (g_nvc2_msg_status & NVC2_STATUS_TRACKING) > 0; }

static inline void nvc2_message_set_tracking()
	{ g_nvc2_msg_status = NVC2_STATUS_ENABLED | NVC2_STATUS_TRACKING; }

static inline void nvc2_message_unset_tracking() {
	g_nvc2_msg_status &= (0xFF ^ NVC2_STATUS_TRACKING);
}


static inline bool nvc2_message_ignore()
	{ return (g_nvc2_msg_status & NVC2_STATUS_IGNORE) > 0; }

static inline void nvc2_message_set_ignore()
	{ g_nvc2_msg_status |= NVC2_STATUS_IGNORE; }

static inline bool nvc2_message_invalid()
	{ return (g_nvc2_msg_status & NVC2_QUERY_INVALID) > 0; }

static inline void nvc2_message_set_invalid()
	{ g_nvc2_msg_status |= NVC2_QUERY_INVALID; }

static inline bool nvc2_message_repeated()
	{ return (g_nvc2_msg_status & NVC2_QUERY_REPEATED) > 0; }

	static inline void nvc2_message_set_repeated()
	{ g_nvc2_msg_status |= NVC2_QUERY_REPEATED; }

static inline bool nvc2_message_read()
	{ return (g_nvc2_msg_status & NVC2_QUERY_READ) > 0; }

static inline void nvc2_message_set_read()
	{ g_nvc2_msg_status |= NVC2_QUERY_READ; }

static inline void nvc2_message_set_command()
	{ g_nvc2_msg_status |= NVC2_QUERY_COMMAND;  }


static inline void nvc2_query_set_available()
	{ g_nvc2_msg_status |= NVC2_STATUS_AVAIL; }

	

bool nvc2_proc_input(u08_t value) {
	if (!nvc2_message_tracking()) {
		// not currently tracking a message, check for message start char
		switch (value) {
			case ASCII_SYN:
				nvc2_message_set_ignore();
			case ASCII_DLE:
				nvc2_message_set_tracking();
				g_nvc2_track = 0;
				g_nvc2_le = 0xFF;    // initial LE
				g_nvc2_crc = 0xFFFF; // initialize CRC for FCS-16
				break;
			default: return false;
		}
				
		// if buffers still blocked by last query, ignore message
		if (nvc2_query_available()) nvc2_message_set_ignore();
	} else {
		// invert if crc value
		if (g_nvc2_track >= g_nvc2_le) value ^= 0xFF;
		// update crc
		g_nvc2_crc = crc16_ccitt_lsbf_update(value, g_nvc2_crc);
	
		if (g_nvc2_track == 0) {
			// LE
			g_nvc2_le = value;
			// If message too long, ignore it
			if (g_nvc2_le > NVC2_BUFFER_SIZE + 5) nvc2_message_set_ignore();
		} else if (g_nvc2_track == 1) {
			// LEr, if != LE, drop message
			if (value != g_nvc2_le) {
				nvc2_message_drop();
			}
		} else if (g_nvc2_track == 2) {
			// RRRRNNN1
			if ((value & 0x01) > 0) {
				// if not master message, ignore it.
				nvc2_message_set_ignore();
			}
			else {
			// check if repeated message
				value = (value >> 1) & 0x07;
				g_nvc2_msgid = value;
				
				if (value == g_nvc2_lastid) nvc2_message_set_repeated();
			}
		} else if (g_nvc2_track == 3) {
			// AAAAAAA0
			// check if read query.
			if ((value & 0x01) > 0) {
				nvc2_message_set_read();
				// initialize response length with 0
				g_nvc2_data[0] = 0;
			}
			// ignore message if not addressed to this system and not
			// general call / broadcast
			value = value >> 1;
			if (value != g_nvc2_address) {
				nvc2_message_set_ignore();
			}
		} else if (g_nvc2_track == g_nvc2_le) {
			// CRC-L
		} else if (g_nvc2_track == g_nvc2_le+1) {
			// CRC-H, full message received
			if (!nvc2_message_ignore()) {
				if (g_nvc2_crc == 0) {
					// valid message
					// compute message data size
					g_nvc2_dsize = (g_nvc2_le>5) ? g_nvc2_le-5 : 0;
				} else {
					// invalid crc
					nvc2_message_set_invalid();
				}
				nvc2_query_set_available();
				// copy tracked message status bits to query status bits
				g_nvc2_query_status = g_nvc2_msg_status;
			}
			// stop tracking
			nvc2_message_unset_tracking();
			// update last-message id
			g_nvc2_lastid = g_nvc2_msgid;
		} else if (g_nvc2_track == 4) {
			// CMD
			if (!nvc2_message_ignore()) {
				nvc2_message_set_command();
				g_nvc2_command = value;
			}
		} else if (!nvc2_message_ignore() && !nvc2_message_repeated()) {
			// DATA
			g_nvc2_data[g_nvc2_track - 5] = value;
		}
		++g_nvc2_track;
	}

	return true;
}


static inline void nvc2_send_response() {
	u08_t msgLen = g_nvc2_query_rsize + 3;
	u16_t crc = 0xFFFF;

	// send SYN

	uart_write_byte(ASCII_SYN);

	// send LE, LEr/LEi, 0000NNN1 and DATA

	u08_t value = msgLen;
	for (size8_t i=0; i<msgLen; ++i) {
		if (i==2) value = ((g_nvc2_lastid & 0x07) << 1) | 0x01;
		if (i>=3) value = g_nvc2_data[i-3];
	
		crc = crc16_ccitt_lsbf_update(value, crc);
		uart_write_byte(value);

		if (!g_nvc2_query_success) value = value ^ 0xFF; // invert repeated length if error
	}
	crc ^= 0xFFFF;

	// send CRC (FCS-16)

	uart_write_byte((u08_t)(crc & 0xFF));
	uart_write_byte((u08_t)(crc >> 8));
}


// 7976, 7948
static inline void nvc2_process_query() {
	if (nvc2_query_invalid()) {
		// invalid query message (transmission error), set empty error response
		g_nvc2_query_rsize = 0;
		g_nvc2_query_success = false;
	} else if (nvc2_query_repeated()) {
		// repeated query, do no processing, leave last response
	} else {
		// normal query
		
		g_nvc2_query_rsize = 0;
		g_nvc2_query_success = false;

		const u08_t command = nvc2_query_command();

		if (!nvc2_query_hascmd()) {
			// process an empty query, set empty response
			g_nvc2_query_success = true;
		} else if (nvc2_query_read()) {
			// process a read query
			const u08_t respSize = nvc2_query_rsize();
			u08_t *data = g_nvc2_data;
			
			if (respSize <= NVC2_BUFFER_SIZE) switch (command) {
				case NVC2_CMD_RWFILE: {
					if (g_nvc2_file_open == NVC2_FILE_FIRMWARE) {
						u08_t *addr = nvmfile_get_base();
						addr += g_nvc2_file_pos;
						
						for (size8_t i=0; i<respSize; ++i) {
							data[i] = nvmfile_read08(addr++);
							++g_nvc2_file_pos;
						}
						
						g_nvc2_query_rsize = respSize;
						g_nvc2_query_success = true;
					}					
				} break;
				case NVC2_CMD_RUNLVL: {
					if (respSize==1) {
						data[0] = g_nvm_runlevel;
						g_nvc2_query_rsize = 1;
						g_nvc2_query_success = true;
					}
				} break;
			}
		} else {
			// process a write query
			const u08_t dsize = nvc2_query_dsize();
			u08_t *data = g_nvc2_data;
			
			const u08_t data0 = data[0];

			switch (command) {
				case NVC2_CMD_FOPEN: {
					if ((dsize==1) && (data0 <= NVC2_MAX_FID)) {
						g_nvc2_file_open = data[1];
						g_nvc2_file_pos = 0;
						g_nvc2_query_success = true;
					}
				} break;
				case NVC2_CMD_FCLOSE: {
					g_nvc2_file_open = -1;
					g_nvc2_query_success = true;
				} break;
				case NVC2_CMD_RWFILE: {
					if (g_nvc2_file_open == NVC2_FILE_FIRMWARE) {
						// opening firmware for writing implies conf runlevel
						g_nvm_runlevel = NVM_RUNLVL_CONF;

						u08_t *addr = nvmfile_get_base();
						addr += g_nvc2_file_pos;
						
						for (size8_t i=0; i<dsize; ++i) {
							nvmfile_write08(addr++, data[i]);
							++g_nvc2_file_pos;
						}
						g_nvc2_query_success = true;
					}
				} break;
				case NVC2_CMD_RUNLVL: {
//if (((1<<data0) & ((1<<NVM_RUNLVL_HALT)|(1<<NVM_RUNLVL_CONF)|(1<<NVM_RUNLVL_VM)|(1<<NVM_RUNLVL_RESET))) > 0)
					if ( (dsize==1) && ( (data0==NVM_RUNLVL_HALT)
						 || (data0==NVM_RUNLVL_CONF)
						 || (data0==NVM_RUNLVL_VM)
						 || (data0==NVM_RUNLVL_RESET) ) )
						{ g_nvm_runlevel = data0;
						  g_nvc2_query_success = true; }
				} break;
			}
		}
	
		if (!g_nvc2_query_success) {
			g_nvc2_query_rsize = 1;
			g_nvc2_data[0] = 1;
		}
	}

	nvc2_send_response();
	nvc2_query_finished();

	// check for reset or halt runlevel
	if (g_nvm_runlevel == NVM_RUNLVL_RESET) nvm_reset();
	if (g_nvm_runlevel == NVM_RUNLVL_HALT)  nvm_halt();
}


void nvc2_check_input() {
	while (true) {
		// wait for input within timeout limits
		for (u08_t i=0; !uart_available() && (i < NVC2_RECV_UART_TIMEOUT); ++i)
			{ delay(MILLISEC(1)); }
		
		if (uart_available()) {
			// input available
			u08_t value = uart_read_byte();
			if (!nvc2_enabled() || !nvc2_proc_input(value)) {
				// received input doesn't belong to a message,
				// possible alternative input processing here
			}
		} else {
			// UART timeout
			nvc2_message_drop();
			break;
		}
		
		if (nvc2_query_available()) {
			// query received
			nvc2_process_query();
			break;
		}
	}
}


#endif // NVMCOMM2
