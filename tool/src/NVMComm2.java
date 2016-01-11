//
//  NVMComm2, the NanoVM Communication Protocol
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
//  Parts of this tool are based on public domain code written by Kimberley
//  Burchett: http://www.kimbly.com/code/classfile/
//

import java.io.*;
import java.util.*;


class NVMComm2 {
	// ASCII codes used during transmission 
	public static final int ASCII_DLE = 0x10;
	public static final int ASCII_SYN = 0x16;

	protected static final int NVC2_TIMEOUT_CHARS = 20; //!!8;      // Input timout in units of byte transmission time
	protected static final int NVC2_MIN_RESPONSE_SIZE = 5;  // Minimum size of a message (without start byte)
	protected static final int NVC2_MAX_MSG_LEN = 0x7F;     // Maximum message length

	protected static final int MAX_DATA_LEN = 16;      // Maximum data length to use in message

	protected InputStream m_inputStream;
	protected OutputStream m_outputStream;
	protected int m_ioSpeed;
	protected int m_recvInputTimeout;
	protected int m_lastMsgID;

	public static final short CMD_NONE    = 0xFF;
	public static final short CMD_FOPEN   = 0x70;
	public static final short CMD_FCLOSE  = 0x71;
	public static final short CMD_RWFILE  = 0x72;
	public static final short CMD_RUNLVL  = 0x7E;

	public static final byte RUNLVL_HALT   = 0x00;
	public static final byte RUNLVL_CONF   = 0x01;
	public static final byte RUNLVL_VM     = 0x03;
	public static final byte RUNLVL_RESET  = 0x06;
	public static final byte RUNLVL_BOOT   = 0x07;

	
	// CCITT CRC-16, msb-first, int version
	// 0 <= data <= 0xFF, 0 <= crc <= 0xFFFF
	public static int crc16ccittUpdateMsbf(int data, int crc) {
		crc  = ((crc >> 8) & 0xFF) | ((crc << 8) & 0xFFFF);
		crc ^= data;
		crc ^= (crc & 0xFF) >> 4;
		crc ^= ((crc << 8) << 4) & 0xFFFF;
		crc ^= ((crc & 0xFF) << 4) << 1;
		return crc;
	}
	
	// CCITT CRC-16, msb-first, byte/short version
	public static short crc16ccittUpdateMsbf(byte data, short crc) {
		return (short)crc16ccittUpdateMsbf(0xFF & (int)data, 0xFFFF & (int)crc);
	}

	// CCITT CRC-16, lsb-first, int version
	// 0 <= data <= 0xFF, 0 <= crc <= 0xFFFF
	public static int crc16ccittUpdateLsbf(int data, int crc) {
		data ^= (crc & 0xFF);
		data ^= (data << 4) & 0xFF;
	
		crc  = ((data << 8) & 0xFFFF) | (crc >> 8);
		crc ^= (data >> 4);
		crc ^= (data << 3) & 0xFFFF;

		return crc;
	}
	
	// CCITT CRC-16, lsb-first, byte/short version
	public static short crc16ccittUpdateLsbf(byte data, short crc) {
		return (short)crc16ccittUpdateLsbf(0xFF & (int)data, 0xFFFF & (int)crc);
	}

	
	protected void put(int i) {
		byte[] ioArray = new byte[1];
		
		i &= 0xff;
		ioArray[0] = (byte)i;
		
		try {
			m_outputStream.write(ioArray);
		} catch (Exception e) {
			System.out.println("Writing Error");
			System.out.println(e.toString());
			System.exit(-1);
		}
	}
	
	
	public static class QueryMessage {
		protected short m_address;
		protected boolean m_read;
		protected short m_command;
		protected byte[] m_data;
	
		public short getAddress() { return m_address; }  // slave address, must be 0x01 for now
		public boolean isRead() { return m_read; }       // read or write
		public short getCommand() { return m_command; }  // 0x00 if empty mesage, 0x00 <= command <= 0xFF
		public byte[] getData() { return m_data; }       // data (only when write query)
		
		public QueryMessage(short address, boolean read, short command,
					 byte[] data)
		{
				m_address = address; m_read = read;
				m_command = command; m_data = data;
		}
	}
	
	
	public static class ResponseMessage {
		public boolean m_error;
		protected byte[] m_data;
	
		public boolean isError() { return m_error; } // error Response / normal Resonse
		public byte[] getData() { return m_data; }   // response data or error info
		
		public ResponseMessage(boolean isError, byte[] data) {
				m_error = isError; m_data = data;
		}
	}
	
	
	
	
	protected static class CommBuffer {
		protected byte[] m_data;
		protected int m_fill;
		
		int size() { return m_data.length; }
	
		int fill() { return m_fill; }
	
		int space() { return size() - fill(); }
	
		byte getData(int i) { return m_data[i]; }
	
		void pushBack(byte b) {	if (space() > 0) m_data[m_fill++] = b; }
	
		void popFront(int count) {
			if (count >= m_fill) m_fill = 0;
			else {
				for (int i=0; i < m_fill - count; ++i) m_data[i] = m_data[i+count];
				m_fill -= count;
			}
		}
		
		public CommBuffer(int maxSize) {
			m_data = new byte[maxSize];
			m_fill = 0;
		}
	}
	
	
	public ResponseMessage receiveResponse(int messageId, CommBuffer buffer) {
		final int NVC2_RECV_DROP   = 0;
		final int NVC2_RECV_SEARCH = 1;
		final int NVC2_RECV_SYNCED = 2;
		final int NVC2_RECV_DATA   = 3;
	
		int status = NVC2_RECV_SEARCH;
		int neededInput = 0;
		boolean commTimeout = false;
	
		boolean errorMsg = false;
	
		int msgLen = 0;
		int recvId = 0;
		
		// Minimum buffer size
		if ( buffer.space() < NVC2_MIN_RESPONSE_SIZE ) return null; 
	
		// Don't give up while input available or while buffer might contain
		// a complete message.
		while ( (! commTimeout) || (buffer.fill() >= NVC2_MIN_RESPONSE_SIZE) ) {
			// if not enough buffer space, drop currently tracked message
			if (buffer.space() < neededInput) status = NVC2_RECV_DROP;
	
			if (status == NVC2_RECV_DROP) {
				// drop currently tracked message
				neededInput = buffer.fill() < 1 ? 1 : 0;
				status = NVC2_RECV_SEARCH;
			}
	
			// try to read necessary input from input stream
			while ( (neededInput > 0) && !commTimeout) {
				// wait for input within timeout limits
				boolean inputAvailable = false;
				try {
					for (int i=0; (m_inputStream.available() == 0) && (i < m_recvInputTimeout); ++i)
						Thread.currentThread().sleep(2); // sleep 2 ms}
					inputAvailable = (m_inputStream.available() > 0);
				} catch(Exception e) {}
				
				if (inputAvailable) {
					// input available
					byte[] data = new byte[1];
					try {
						m_inputStream.read(data);
					} catch (Exception e) {
						System.out.println("Reading Error");
						System.out.println(e.toString());
						System.exit(-1);
					}
// System.out.print(" " + Integer.toHexString(0xFF & (int)data[0]) ); //!! DEBUG
					buffer.pushBack(data[0]);
	
					if (neededInput > 0) --neededInput;
				} else {
					// timeout
					commTimeout = true;
				}
			}
			
			// if not enough data after reading, drop currently tracked message
			if (neededInput > 0) status = NVC2_RECV_DROP;
			
			if (status == NVC2_RECV_SEARCH) {
				// need at least 1 byte of input
				neededInput = (buffer.fill() < 1) ? 1 : 0;
				if (neededInput == 0) {
					
					// search buffer for slave-message start-symbol
					for (int i=0; i < buffer.fill(); ++i) {
						byte value = buffer.getData(0);
						buffer.popFront(1);
					
						if ((0xFF & (int)value) == ASCII_SYN)
							{ status = NVC2_RECV_SYNCED; break; }
					}
				}
			}
			
			if (status==NVC2_RECV_SYNCED) {
				// need first 4 bytes of input
				neededInput = (buffer.fill() < 4) ?
					4 - buffer.fill() : 0;
				if (neededInput == 0) {
					int msgLenR;
					
					// analyse message header
					
					msgLen = (0xFF) & (int) buffer.getData(0);
					int MsgLen2 = 0xFF & (int) buffer.getData(1);
					int MsgLen2inv = 0xFF ^ MsgLen2;
					
					if (msgLen != MsgLen2inv)
						msgLenR = MsgLen2;
					else {
						// LEi instead of LEr, signifies error message
						errorMsg = true;
						msgLenR = MsgLen2inv;
					}
					
					recvId = 0xFF & (int) buffer.getData(2);
					boolean slaveFlag = (recvId & 0x01) > 0;
					recvId = (recvId >> 1) & 0x07;
	
					if ((msgLen != msgLenR) || !slaveFlag) {
						status = NVC2_RECV_DROP;
					} else {
						status = NVC2_RECV_DATA;
					}
				}
			}
			
			if (status==NVC2_RECV_DATA) {
				// need complete message
				neededInput = (buffer.fill() < msgLen+2) ?
					msgLen+2 - buffer.fill() : 0;
				if (neededInput == 0) {
					// calculate CRC (FCS-16)
					short crc = (short) 0xFFFF;
					for (int i=0; i<msgLen; ++i)
						crc = crc16ccittUpdateLsbf(buffer.getData(i), crc);
					crc ^= 0xFFFF;
					
					short readCrc = (short) (
						  ( ( 0xFF & (int)buffer.getData(msgLen) ) << 0 )
						| ( ( 0xFF & (int)buffer.getData(msgLen+1) ) << 8 )
					);
					
					if ( crc!=readCrc ) {
						// System.out.println("DEBUG: CRC error, was: "
						//	+ Integer.toHexString(0xFFFF & (int)readCrc)
						//	+ " should have been: "
						//	+ Integer.toHexString(0xFFFF & (int)crc)
						//);
						// crc error
						return new ResponseMessage(true, null);
					} else {
						if (recvId == messageId) {
							// generate and return message
							byte[] data = null;
							if (msgLen > 3) {
								data = new byte[msgLen - 3];
								for (int i=0; i<msgLen - 3; ++i)
									data[i] = buffer.getData(i+3);
							}
		
							return new ResponseMessage(errorMsg, data);
						} else {
							// wrong message number, drop message
							status = NVC2_RECV_DROP;
						}
					}
				}
			}
		}
	
		return null;
	}
	
	
	public void sendQuery(int messageId, QueryMessage query) {
		int dsize = (query.getData() == null) ? 0 : query.getData().length;
		if (dsize >= NVC2_MAX_MSG_LEN - 5) return; // Can't fit data in response
		
		boolean sendContent = query.getCommand() != CMD_NONE;
		int msgLen = query.getCommand() == CMD_NONE ? 4 : dsize + 5;
	
		// send DLE
		put(ASCII_DLE);
	
		// compose message
		byte[] msg = new byte[msgLen+2];
		msg[0] = (byte)msgLen;
		msg[1] = (byte)msgLen;
		msg[2] = (byte)( (0x07 & messageId) << 1 );
		msg[3] = (byte)( ((0xFF & (int)query.getAddress()) << 1) | (query.isRead() ? 1 : 0) );
		
		if (sendContent) {
			msg[4] = (byte) query.getCommand();
			for (int i=0; i<dsize; ++i) msg[i+5] = query.getData()[i];
		}
		
		// calculate and append CRC (FCS-16)
		short crc = (short) 0xFFFF;
		for (int i=0; i<msgLen; ++i)
			crc = crc16ccittUpdateLsbf(msg[i], crc);
		crc ^= 0xFFFF;
		msg[msgLen+0] = (byte)( 0xFF & (crc >> 0) );
		msg[msgLen+1] = (byte)( 0xFF & (crc >> 8) );
	
// System.out.print("DEBUG: Sending query:     ");
// System.out.print(" " + Integer.toHexString(0xFF & (int)ASCII_DLE));
// for (int i=0; i<msgLen+2; ++i) System.out.print(" " + Integer.toHexString(0xFF & (int)msg[i]));
// System.out.println("");

		// send message
		for (int i=0; i<msgLen+2; ++i) put(msg[i]);
	}
	
	
	public ResponseMessage executeQuery(short address, boolean read,
		short command, byte[] data, int maxTries)
	{
		final int recvBufferSize = 0xFF;
		
		m_lastMsgID = 0x07 & (m_lastMsgID + 1);
		int messageId = m_lastMsgID;
		QueryMessage query = new QueryMessage(address, read, command, data);
		CommBuffer buffer = new CommBuffer(recvBufferSize);
		
		for (int i=0; (maxTries==0) || (i<maxTries); ++i) {
			sendQuery(messageId, query);
			
// System.out.print("DEBUG: Receiving response:");
			ResponseMessage response = receiveResponse(messageId, buffer);
// System.out.println("");
			if (response != null) {
				if (!response.isError()) {
					// normal message
// System.out.println("DEBUG: Received normal response.");
					return response;
				} else {
					// error message, try again if transmission error, else return
					if ((response.getData() != null) && (response.getData().length>0)) {
// System.out.println("DEBUG: Received error response with error code.");
						return response;
					} else {
// System.out.println("DEBUG: Received transmission error.");
					}
				}
			} else {
// System.out.println("DEBUG: Received no valid response.");
				// timeout, do nothing
			}
		}

// System.out.println("DEBUG: Communication timeout.");

		return null;
	}


	public ResponseMessage executeQuery(short address, boolean read,
		short command, byte data, int maxTries)
	{
		byte[] dataArray = {data};
		return executeQuery(address, read, command, dataArray, maxTries);
	}

	
	public NVMComm2(InputStream inputStream, OutputStream outputStream, int ioSpeed) {
		m_inputStream = inputStream;
		m_outputStream = outputStream;
		m_ioSpeed = ioSpeed;
		m_recvInputTimeout = NVC2_TIMEOUT_CHARS*(10000/ioSpeed) + 1;
		m_lastMsgID = 0;
	}
}
