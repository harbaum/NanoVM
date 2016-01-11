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

import java.io.*;
import java.util.*;

public class Config {
  public static final int TARGET_NONE       = -1;
  public static final int TARGET_FILE       = 0;
  public static final int TARGET_NVC1_AUTO  = 2;  // = UART for now
  public static final int TARGET_NVC1_UART  = 2;
  public static final int TARGET_NVC1_ASURO = 3;
  public static final int TARGET_NVC2_AUTO  = 4;
  public static final int TARGET_CTBOT_AUTO  = 5;

  static NativeMapper nativeMapper = new NativeMapper();

  // the config from file
  static int maxSize = 0;
  static String configName = null;

  static int target = TARGET_NONE;
  static String targetFile = null;
  static int targetSpeed = -1;

  static public int getTarget() {
    return target;
  }

  static public String getFileName() {
    return targetFile;
  }

  static public void overwriteFileName(String name) {
    targetFile = name;
  }

  static public int getSpeed() {
    return targetSpeed;
  }

  static public int getMaxSize() {
    return maxSize;   // asuro
  }

  static public void load(String fileName) {
    System.out.println("Read config " + fileName);

    File inputFile = new File(fileName);

    // get config path
    String configPath = inputFile.getParent();
    if(configPath == null)
      configPath = ".";

    // Object is always native
    nativeMapper.load(configPath + File.separator + "Object");

    try {
      String line;
      FileInputStream in = new FileInputStream(inputFile);
      BufferedReader reader = new BufferedReader
	(new InputStreamReader(in));
   
      // read through all lines in file
      while((line = reader.readLine()) != null) {  
	StringTokenizer st = new StringTokenizer(line);
	boolean skipRest = false;
	int token_index = 0;
	String name=null, value=null;

	while(st.hasMoreTokens() && !skipRest) {
	  String token = st.nextToken();

	  if(token.charAt(0) == '#') 
	    skipRest = true;
	  else {
	    if(token_index == 0)      name = token;
	    else if(token_index == 1) value = token;
	  }
	  
	  token_index++;
	}

	if(name != null) {
	  if(name.equalsIgnoreCase("maxsize") && (value != null)) 
	    maxSize = Integer.parseInt(value);
	  else if(name.equalsIgnoreCase("name") && (value != null)) 
	    configName = value;
	  else if(name.equalsIgnoreCase("native") && (value != null)) 
	    nativeMapper.load(configPath + File.separator + value);
	  else if(name.equalsIgnoreCase("target") && (value != null)) {
	    if(value.equalsIgnoreCase("file"))
	      target = TARGET_FILE;
	    else if(value.equalsIgnoreCase("nvmcomm1/auto"))
	      target = TARGET_NVC1_AUTO;
	    else if(value.equalsIgnoreCase("nvmcomm1/uart"))
	      target = TARGET_NVC1_UART;
	    else if(value.equalsIgnoreCase("uart")) // legacy support
	      target = TARGET_NVC1_UART;
	    else if(value.equalsIgnoreCase("nvmcomm1/asuro"))
	      target = TARGET_NVC1_ASURO;
	    else if(value.equalsIgnoreCase("asuro")) // legacy support
	      target = TARGET_NVC1_ASURO;
	    else if(value.equalsIgnoreCase("nvmcomm2/auto"))
	      target = TARGET_NVC2_AUTO;
            else if(value.equalsIgnoreCase("ctbot"))
              target = TARGET_CTBOT_AUTO;
	    else {
	      System.out.println("ERROR: Unknown target \"" + value + "\"");
	      System.exit(-1);
	    }
	  } else if(name.equalsIgnoreCase("filename") && (value != null)) {
	    targetFile = value;
	  } else if(name.equalsIgnoreCase("speed") && (value != null)) {
	    targetSpeed = Integer.parseInt(value);
	  } else {
	    System.out.println("ERROR: Unknown config entry \"" + name + "\"");
	    System.exit(-1);
	  }
	}
      }
    
      in.close();
    } catch(IOException e) {
      System.out.println("Error reading map");
      System.out.println(e.toString());
      System.exit(-1);
    }
  }
}
