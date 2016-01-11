//
//  NanoVMTool, Converter and Upload Tool for the NanoVM
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
//  Parts of this tool are based on public domain code written by Kimberley
//  Burchett: http://www.kimbly.com/code/classfile/
//

//
// UVMWriter.java
//

import java.io.*;
import java.util.*;

public class UVMWriter {
  static final int MAGIC   = 0xBE000000;
  static final int VERSION = 2;

  byte[] outputBuffer;
  int cur;

  // write a 8 bit value into buffer and make sure buffer
  // end is not overwritten
  void write8(int val) throws ConvertException {
    if(cur >= Config.getMaxSize()) 
      throw new ConvertException("Converted data too big");

    outputBuffer[cur++] = (byte)val;
  }

  // write a 16 bit value little endian into buffer
  void write16(int val) throws ConvertException {
    write8(val&0xff);
    write8((val>>8)&0xff);
  }

  // write a 32 bit value little endian into buffer
  void write32(int val) throws ConvertException {
    write8(val&0xff);
    write8((val>>8)&0xff);
    write8((val>>16)&0xff);
    write8((val>>24)&0xff);
  }

  void updateHeader() throws ConvertException {
    int old_cur=cur;
    cur = 0;
    writeHeader();
    cur = old_cur;
  }

  // write uvm file header
  void writeHeader() throws ConvertException {
    int offset = 15;    // header size: 15 bytes

    write32(MAGIC|UsedFeatures.get());
    write8(VERSION);
    write8(ClassLoader.totalMethods());
    write16(ClassLoader.getMainIndex());

    // offset to constant data
    offset += 2 * ClassLoader.totalClasses(); // class header size: 2bytes
    write16(offset);
    
    // offset to string data
    offset += 4 * ClassLoader.totalConstantEntries(); // constant value size: 4bytes
    write16(offset);

    // offset to method data
    offset += 2 * ClassLoader.totalStrings(); // string indices
    offset += ClassLoader.totalStringSize();  // string data
    write16(offset);
    write8(ClassLoader.totalStaticFields());  // static fields
  }

  // write all class headers
  void writeClassHeaders() throws ConvertException {
    for(int i=0;i<ClassLoader.totalClasses();i++) {
      ClassInfo classInfo = ClassLoader.getClassInfo(i);

      write8(classInfo.getSuperClassIndex());
      write8(classInfo.nonStaticFields());
    }
  }

  // write all 32bit constant values
  void writeConstantEntries() throws ConvertException {
    System.out.println("Writing " + ClassLoader.totalConstantEntries() + " constant entries");
    for(int i=0;i<ClassLoader.totalConstantEntries();i++) {
      System.out.println("  entry[" + i + "] = 0x" + Integer.toHexString(ClassLoader.getConstantEntry(i)));
      write32(ClassLoader.getConstantEntry(i));
    }
  }

  // write all string headers and data 
  void writeStrings() throws ConvertException {
    System.out.println("Writing " + ClassLoader.totalStrings() + " strings");

    // write array of string offsets
    int offset = 2 * ClassLoader.totalStrings();
    for(int i=0;i<ClassLoader.totalStrings();i++) {
      write16(offset);
      offset += ClassLoader.getString(i).length()+1;
    }

    // write the strings itself
    for(int i=0;i<ClassLoader.totalStrings();i++) {
      String str = ClassLoader.getString(i);
      System.out.println("  entry[" + (i+ClassLoader.totalConstantEntries()) + "] = \"" + str+ "\"");

      // write zero terminated c strings
      for(int j=0;j<str.length();j++) write8(str.charAt(j));
      write8(0);
    }
  }

  // write all methods
  void writeMethods() throws ConvertException {
    int codeOffset = 0;

    // build the method id table
    MethodIdTable.build();
      
    // write all Method headers
    for(int i=0;i<ClassLoader.totalMethods();i++) {
      MethodInfo methodInfo = ClassLoader.getMethod(i);
      
      // offset from this header to bytecode (this header is 8 bytes
      // in size)
      write16((ClassLoader.totalMethods()-i)*8+codeOffset);      // code_index 
      write16((ClassLoader.getClassIndex(i) << 8) + 
	      MethodIdTable.getEntry(i));                        // id
      write8(methodInfo.getName().equals("<clinit>")?1:0);       // flags
      write8(methodInfo.getArgs());                              // args
      write8(methodInfo.getCodeInfo().getMaxLocals());           // max_locals
      write8(methodInfo.getCodeInfo().getMaxStack());            // max_stack
      
      codeOffset += methodInfo.getCodeInfo().getBytecode().length;
    }

    // write bytecode
    for(int i=0;i<ClassLoader.totalMethods();i++) {
      ClassInfo classInfo = ClassLoader.getClassInfoFromMethodIndex(i);
      MethodInfo methodInfo = ClassLoader.getMethod(i);

      System.out.println("Converting " + 
			 classInfo.getName() + "." +
			 methodInfo.getName() + ":" +
			 methodInfo.getSignature());

      byte code[] = ClassLoader.getMethod(i).getCodeInfo().getBytecode();

      // adjust references etc
      CodeTranslator.translate(classInfo, code);

      // and write bytecode
      for(int j=0;j<code.length;j++)
	write8(code[j]);

      System.out.println(""); 
    }
  }

  public UVMWriter(boolean writeHeader) {
    System.out.println("Generating unified class file ...");

    // create output buffer and reset output pointer
    outputBuffer = new byte[Config.getMaxSize()];
    cur = 0;

    try {
      writeHeader();           // write file header
      writeClassHeaders();     // write class headers
      writeConstantEntries();  // write all 32-bit constants
      writeStrings();          // write all string data
      writeMethods();          // write method headers and byte code
      updateHeader();          // update feature values
      
      // overwrite target config when -c option was given
      if(writeHeader) {
	System.out.println("Writing C header file");

	String fileName = ClassLoader.getClassInfo(0).getName()+".h";
	// if a file name has been specified in the Config file or via the
	// command line use that
	if(Config.getFileName() != null)
	  fileName = Config.getFileName();

	// write header file to disk
	try {
	  System.out.println("Writing to file " + fileName);
	  File outputFile = new File(fileName);
	  FileOutputStream out = new FileOutputStream(outputFile);

	  out.write(("/* " + fileName + ", autogenerated from " + 
		     ClassLoader.getClassInfo(0).getName() + 
		     " class */\n").getBytes());

	  out.write("{\n".getBytes());

	  for(int i=0;i<cur;i++) {
	    String str = "0" + Integer.toHexString(0xff&(int)outputBuffer[i]);
	    out.write(("0x" + str.substring(str.length()-2)).getBytes());

	    if(i < cur-1)  out.write(",".getBytes());
	    if(i%16 == 15) out.write("\n".getBytes());
	    else           out.write(" ".getBytes());
	  }

	  out.write("\n};\n".getBytes());
	  out.close();
	} catch(IOException e) {
	  System.out.println("Error writing header file: " + e.toString());
	  System.exit(-1);
	}
	
      } else {

	// do with result what config says
	int target = Config.getTarget();
	switch(target) {
	  case Config.TARGET_NONE:
	    System.out.println("ERROR: no target specified");
	    System.exit(-1);
	    break;
	    
	  case Config.TARGET_FILE:
	    // write file to disk
	    try {
	      String fileName = ClassLoader.getClassInfo(0).getName()+".nvm";
	      // if a file name has been specified in the Config file or 
	      // via the command line use that
	      if(Config.getFileName() != null)
		fileName = Config.getFileName();
	      
	      System.out.println("Writing to file " + fileName);
	      File outputFile = new File(fileName);
	      FileOutputStream out = new FileOutputStream(outputFile);
	      out.write(outputBuffer, 0, cur);
	      out.close();
	    } catch(IOException e) {
	      System.out.println("Error writing file: " + e.toString());
	      System.exit(-1);
	    }
	    break;
	    
	  // case Config.TARGET_NVC1_AUTO:
	  case Config.TARGET_NVC1_UART:
	  case Config.TARGET_NVC1_ASURO:
            Uploader.setUploader("nvc1");
            Uploader.upload(Config.getFileName(), target, 
                            Config.getSpeed(), outputBuffer, cur);
	  case Config.TARGET_NVC2_AUTO:
            Uploader.setUploader("nvc2");
	    Uploader.upload(Config.getFileName(), target, 
			    Config.getSpeed(), outputBuffer, cur);
	    break;
          
          case Config.TARGET_CTBOT_AUTO:
            Uploader.setUploader("ctbot");
            Uploader.upload(Config.getFileName(), target, 
                            Config.getSpeed(), outputBuffer, cur);
            break;
            
	}
      }
    } catch(ConvertException e) {
      System.out.println("Conversion failed: " + e.toString());
      System.exit(-1);
    }
  }
}
