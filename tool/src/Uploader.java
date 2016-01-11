//
//  NanoVMTool, Converter and Upload Tool for the NanoVM
//  Copyright (C) 2005-2006 by Till Harbaum <Till@Harbaum.org>
//                             Oliver Schulz <whisp@users.sf.net>
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
import gnu.io.*;  // needs to be added to vm

public class Uploader {

  // ASCII codes used during transmission 
  final static int ASCII_SOH = 0x01;
  final static int ASCII_STX = 0x02;
  final static int ASCII_EOT = 0x04;
  final static int ASCII_ACK = 0x06;
  final static int ASCII_DLE = 0x10;
  final static int ASCII_XON = 0x11;
  final static int ASCII_XOFF = 0x13;
  final static int ASCII_NAK = 0x15;
  final static int ASCII_SYN = 0x16;
  final static int ASCII_CAN = 0x18;
  final static int ASCII_EOF = 0x1A;

  final static int BLOCK_SIZE = 16;

  // Asure needs specific handling
  static boolean asuro_echo_suppression = false;

  static Enumeration	    portList;
  static CommPortIdentifier portId;
  static SerialPort	    serialPort;
  static OutputStream       outputStream;
  static InputStream        inputStream;
  static boolean	    outputBufferEmptyFlag = false;

  public static void setUploader(String type){
    // ignore for now...
  }

  static int get(boolean timeout) {
    final int TIMEOUT = 50;
    byte[] inputArray = new byte[1];

    if(timeout) {
      int cnt=0;
      try {
	while((inputStream.available() == 0) && (cnt<TIMEOUT)) {
	  Thread.currentThread().sleep(10);  // 10 msec
	  cnt++;
	}
      } catch(Exception e) {}
      
      if(cnt == TIMEOUT) 
	return -1;
    }

    try {
      inputStream.read(inputArray);
    } catch (Exception e) {
      System.out.println("Error reading from serial port");
      System.out.println(e.toString());
      System.exit(-1);
    }
    return inputArray[0];
  }

  static void put(int i) {
    byte[] ioArray = new byte[1];

    i &= 0xff;
    ioArray[0] = (byte)i;

//    System.out.println("sending " + 
//		       Integer.toHexString(((int)ioArray[0])&0xff)+" "+
//		       Integer.toHexString(i));

    try {
      outputStream.write(ioArray);
    } catch (Exception e) {
      System.out.println("Error writing to serial port");
      System.out.println(e.toString());
      System.exit(-1);
    }
    
    if(asuro_echo_suppression) {
      // simple echo cancelation

      try {
	inputStream.read(ioArray);
      } catch (Exception e) {
	System.out.println("Error reading echo from serial port");
	System.out.println(e.toString());
	System.exit(-1);
      }

      if((((int)ioArray[0])&0xff) != i) 
	System.out.println("Echo error: " + Integer.toHexString(i) + "/" + 
			   Integer.toHexString(((int)ioArray[0])&0xff));
    }
  }

  public static void upload(String device, int target,
			    int speed, byte[] file, int length) {
    boolean portFound = false;
    int i, j, block, n;
    int sum;
    boolean done;
    int cur = 0;

    if(target == Config.TARGET_NVC1_ASURO) {
      System.out.println("Enabling Asuro IR echo suppression.");
      asuro_echo_suppression = true;
    }

    portList = CommPortIdentifier.getPortIdentifiers();
    
    while (portList.hasMoreElements()) {
      portId = (CommPortIdentifier) portList.nextElement();
      
      if(portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
	
	if(portId.getName().equals(device)) {
	  System.out.println("Found port " + device);
	  
	  portFound = true;
	  
	  try {
	    serialPort = (SerialPort) portId.open("Uploader", 2000);
	  } catch (PortInUseException e) {
	    System.out.println("Port in use.");
	    
	    continue;
	  } 
	  
	  try {
	    outputStream = serialPort.getOutputStream();
	    inputStream = serialPort.getInputStream();
	  } catch (IOException e) {}
	  
	  try {
	    serialPort.setSerialPortParams(speed, 
		   SerialPort.DATABITS_8, SerialPort.STOPBITS_1, 
		   SerialPort.PARITY_NONE);
	  } catch (UnsupportedCommOperationException e) {}
	  
	  
	  try {
	    serialPort.notifyOnOutputEmpty(true);
	  } catch (Exception e) {
	    System.out.println("Error setting event notification");
	    System.out.println(e.toString());
	    serialPort.close();
	    System.exit(-1);
	  }
	  

if ( (target == Config.TARGET_NVC1_AUTO)
	 || (target == Config.TARGET_NVC1_UART)
     || (target == Config.TARGET_NVC1_ASURO) )
{
// == NVMCOMM1 ========================================================

	  // first flush buffers
	  while((i = get(true)) != -1);

	  System.out.println("Using NanoVM Communication Protocol v1.0");
	  
	  // wait for NAK
	  System.out.println("Please reset/start target now ...");
	  while((i=get(false)) != ASCII_NAK) {
	    System.out.println("Received " + i);
	  }
	  
	  // wait some time for receiver to start
	  try {
	    Thread.currentThread().sleep(100);
	  } catch(Exception e) {}

	  // start sending first block
	  i = ASCII_ACK;
	  block = 0;
	  j = 0;

	  do {
	    if(i == ASCII_ACK) {
	      cur += j;
	      j = length - cur;
	      if(j > BLOCK_SIZE) j = BLOCK_SIZE;

	      block++;
	      if(j > 0)
		System.out.print("Sending block " + block + " with " +
				 j + " bytes ... ");
	    } else
	      System.out.print("Resending block " + block + " ... ");

	    if(j > 0) {

	      // send header
	      put(ASCII_SOH);
	      put(block);
	      put(~block);

	      // send data and calculate sum
	      for(n=0,sum=0;n<j;n++) {
		sum += file[cur+n];
		put(file[cur+n]);
	      }

	      // fill up with zeros if required
	      for(;n<BLOCK_SIZE;n++)
		put(0);

	      // send sum
	      put(sum);

	      // check result
	      while(((i=get(true)) != ASCII_NAK)&&(i != ASCII_ACK)) {
		// ignore timeouts
		if(i>=0)
		  System.out.println("received " + i);
	      }

	      // if we received an ack -> good!!
	      if(i == ASCII_ACK) {
		System.out.println("ok");
		if(j == BLOCK_SIZE) done = false;
		else                done = true;
	      } else {
		System.out.println("error");
		done = false;
	      }
	    } else
	      done = true;  // no more bytes to send
	  } while(!done);
	  
	  // transmission ended, send eof and expect ack
	  put(ASCII_EOF);
	  while(((i=get(true)) != ASCII_NAK)&&(i != ASCII_ACK));

	  if(i == ASCII_ACK) {
	    if(asuro_echo_suppression)
	      System.out.println("Download successful, " + 
				 "please restart your Asuro");
	    else
	      System.out.println("Download successful, " +
				 "program started!");
	  } else
	    System.out.println("Error: nak after eof");


// ====================================================================
} // NVMCOMM1

else if (target == Config.TARGET_NVC2_AUTO)
{
// == NVMCOMM2 ========================================================

	final int maxSearchTries = 10000;
	
	short slaveAddr = 0x10; // default slave address;

	System.out.println("Using NanoVM Communication Protocol v2.0");

//	System.out.println("DEBUG: speed = "+speed);

	NVMComm2 nvc = new NVMComm2(inputStream, outputStream, speed);
	
	boolean slaveFound = false;
	
	System.out.print("Info: Looking for target system");
	for (int tryctr=0; tryctr<maxSearchTries; ++tryctr) {
		NVMComm2.ResponseMessage response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_NONE, null, 1);
		if ((response != null) && !response.isError()) {
			slaveFound = true;
			break;
		}
		try{ Thread.currentThread().sleep(100); } catch(Exception e) {}
		System.out.print(" .");
	}
	System.out.println("");
	
	if (slaveFound) {
		System.out.println("Info: Target system found");
		
		NVMComm2.ResponseMessage response;

		System.out.println("Info: Setting target runlevel to conf.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_RUNLVL, NVMComm2.RUNLVL_CONF, 0);

		System.out.println("Info: Opening target firmware.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_FOPEN, (byte)0, 0);

		for (int filePos=0; filePos < length;) {
			int dlen = NVMComm2.MAX_DATA_LEN < length-filePos ?
				NVMComm2.MAX_DATA_LEN : length-filePos;

			byte[] queryData = new byte[dlen];
			int dataPos = 0;
			while ( (dataPos < queryData.length) && (filePos < length) )
				queryData[dataPos++] = file[filePos++];

			System.out.println("Info: Writing firmware bytes "+Integer.toHexString(filePos-queryData.length)+" to "+Integer.toHexString(filePos-1));
			response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_RWFILE, queryData, 0);

			if ((response == null) || response.isError()) {
				System.out.println("Info: Error writing firmware!");
				break;
			}
		}

		System.out.println("Info: Closing target firmware.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_FCLOSE, (byte)0, 0);

		System.out.println("Info: Setting target runlevel to reset.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_RUNLVL, NVMComm2.RUNLVL_RESET, 0);

		try { Thread.currentThread().sleep(200); } catch(Exception e) {}
		
		System.out.println("Info: Setting target runlevel to conf.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_RUNLVL, NVMComm2.RUNLVL_CONF, 0);
		
		System.out.println("Info: Opening target firmware.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_FOPEN, (byte)0, 0);

		byte[] testFile = new byte[length];
		for (int filePos=0; filePos < length;) {
			int rlen = NVMComm2.MAX_DATA_LEN < length-filePos ?
				NVMComm2.MAX_DATA_LEN : length-filePos;

			System.out.println("Info: Reading firmware bytes "+Integer.toHexString(filePos)+" to "+Integer.toHexString(filePos+rlen-1));
			response = nvc.executeQuery(slaveAddr, true, NVMComm2.CMD_RWFILE, (byte)rlen, 0);

			if ((response == null) || response.isError()) {
				System.out.println("Info: Error reading firmware!");
				break;
			}
			
			for (int pos = 0; pos < response.getData().length; ++pos) {
				testFile[filePos] = response.getData()[pos];
				filePos++;
			}
		}

		System.out.println("Info: Closing target firmware.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_FCLOSE, (byte)0, 0);
		
		System.out.println("Info: Checking target firmware.");
		for (int pos=0; pos<length; ++pos) {
			if (file[pos] != testFile[pos])
				System.out.println("Info: Firmware verify failed at byte 0x"+Integer.toHexString(pos)+" : "+Integer.toHexString(0xFF & (int)testFile[pos])+" != "+Integer.toHexString(0xFF & (int)file[pos]));
		}
		
		System.out.println("Info: Setting target runlevel to halt.");
		response = nvc.executeQuery(slaveAddr, false, NVMComm2.CMD_RUNLVL, NVMComm2.RUNLVL_HALT, 0);
	} else {
		System.out.println("Error: Target system not responding");
	}

// ====================================================================
} // NVMCOMM2
else System.out.println("Error: unknown target");

		
	  serialPort.close();
	  System.exit(0);
	} 
      } 
    } 
    
    if (!portFound) {
      System.out.println("Port " + device + " not found.");
    } 
  } 

  public static void main(String[] args) {
    byte[] buffer = null;

    if(args.length != 3) {
      System.out.println("Usage: Uploader device {bitrate|asuro} file");
      System.exit(-1);
    }
    
    // load file into memory
	// nvmcomm2 not enabled here yet.
    try {
      File inputFile = new File(args[2]);
      buffer = new byte[(int)inputFile.length()];      
      FileInputStream in = new FileInputStream(inputFile);
      in.read(buffer);
      in.close();
    } catch(IOException e) {
      System.out.println("Error loading file " + args[2]);
      System.exit(-1);      
    }

    if(args[1].equalsIgnoreCase("asuro"))
      upload(args[0], Config.TARGET_NVC1_ASURO, 2400, buffer, buffer.length);
    else
      upload(args[0], Config.TARGET_NVC1_UART, Integer.parseInt(args[1]), buffer, buffer.length);
  }
}

