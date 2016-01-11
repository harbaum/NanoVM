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
// NativeMapper.java
//

import java.io.*;
import java.util.*;

public class NativeMapper {
  static private Vector nativeClasses = new Vector();
  static private Vector nativeMethods = new Vector();
  static private Vector nativeFields = new Vector();
  static public int lowestNativeId = 9999;  // lowest native class id

  // java doesn't have native classes, but we do
  class NativeClass {
    String className;
    int id;
  }

  class NativeMethod {
    String className;
    String name;
    String type;
    int id;
  }

  class NativeField {
    String className;
    String name;
    String type;
    int id;
  }

  public NativeMapper() {
    System.out.println("Initializing native mapper");
  }

  public void load(String className) {
    System.out.println("read native " + className);

    System.out.println("trying to load "+ className + ".native");  // XXXX 
    File inputFile = new File(className + ".native");

    try {
      String line;
      String fullClassName = null;
      int fullClassId = 0;

      FileInputStream in = new FileInputStream(inputFile);
      BufferedReader reader = new BufferedReader
	(new InputStreamReader(in));
   
      // read through all lines in file
      while ((line = reader.readLine()) != null) {  
	StringTokenizer st = new StringTokenizer(line);
	boolean skipRest = false;
	int token_cnt = 0;
	String name = null, value = null, id = null;

	while(st.hasMoreTokens() && !skipRest) {
	  String token = st.nextToken();
	  
	  if(token.charAt(0) == '#') 
	    skipRest = true;
	  else {
	    NativeMethod nativeMethod = new NativeMethod();

	    if(token_cnt == 0) name = token;
	    else if(token_cnt == 1) value = token;
	    else if(token_cnt == 2) id = token;
	    else {
	      System.out.println("Ignoring superfluous data: " + token);
	    }
	      
	    token_cnt++;
	  }
	}

	if(token_cnt > 0) {
	  // class entry
	  if(name.equalsIgnoreCase("class") && (token_cnt == 3)) {
	    NativeClass nativeClass = new NativeClass();
	    nativeClass.className = value;
	    nativeClass.id = Integer.parseInt(id);
	    nativeClasses.addElement(nativeClass);
	    
	    if(Integer.parseInt(id) < lowestNativeId)
	      lowestNativeId = Integer.parseInt(id);
	    
	    // save locally for further method processing
	    fullClassName = value;
	    fullClassId = Integer.parseInt(id);
	  }
	  // method entry
	  else if(name.equalsIgnoreCase("method") && (token_cnt == 3)) {
	    if(value.indexOf(':') == -1) {
	      System.out.println("Invalid method reference");
	      System.exit(-1);
	    }
	    
	    if(fullClassName == null) {
	      System.out.println("Method reference before class");
	      System.exit(-1);
	    }
	    
	    // seperate class, method and type
	    NativeMethod nativeMethod = new NativeMethod();
	    nativeMethod.className = fullClassName;
	    nativeMethod.name = value.substring(0, value.indexOf(':'));
	    nativeMethod.type = value.substring(
	      value.indexOf(':')+1, value.length());
	    nativeMethod.id = Integer.parseInt(id) + (fullClassId << 8);
	    
	    nativeMethods.addElement(nativeMethod);
	  }

	  // field entry
	  else if(name.equalsIgnoreCase("field") && (token_cnt == 3)) {
	    if(value.indexOf(':') == -1) {
	      System.out.println("Invalid field reference");
	      System.exit(-1);
	    }
	    
	    if(fullClassName == null) {
	      System.out.println("Method reference before class");
	      System.exit(-1);
	    }
	    
	    // seperate class, method and type
	    NativeField nativeField = new NativeField();
	    nativeField.className = fullClassName;
	    nativeField.name = value.substring(0, value.indexOf(':'));
	    nativeField.type = value.substring(
	      value.indexOf(':')+1, value.length());
	    nativeField.id = Integer.parseInt(id) + (fullClassId << 8);
	    
	    nativeFields.addElement(nativeField);
	  }
	  
	  else {
	    System.out.println("Unknown entry in native file: " + name +
			       " + " + token_cnt + " parms");
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

  public static boolean methodIsNative(String className, 
				       String name, String type) {

    // search through all native methods
    for(int i=0;i<nativeMethods.size();i++) {
      NativeMethod nativeMethod = (NativeMethod)nativeMethods.elementAt(i);
      
      // check if we have a match
      if((className.equals(nativeMethod.className)) &&
	 (name.equals(nativeMethod.name)) &&
	 (type.equals(nativeMethod.type))) 
	
	return true;
    }

    return false;
  }

  public static int getMethodId(String className, 
				String name, String type) {
    
    // search through all native methods
    for(int i=0;i<nativeMethods.size();i++) {
      NativeMethod nativeMethod = (NativeMethod)nativeMethods.elementAt(i);
      
      // check if we have a match
      if((className.equals(nativeMethod.className)) &&
	 (name.equals(nativeMethod.name)) &&
	 (type.equals(nativeMethod.type))) 
	
	return nativeMethod.id;
    }

    return -1;
  }

  public static int getFieldId(String className, 
			       String name, String type) {
    
    // search through all native fields
    for(int i=0;i<nativeFields.size();i++) {
      NativeField nativeField = (NativeField)nativeFields.elementAt(i);
      
      // check if we have a match
      if((className.equals(nativeField.className)) &&
	 (name.equals(nativeField.name)) &&
	 (type.equals(nativeField.type))) 
	
	return nativeField.id;
    }

    return -1;
  }

  public static int getNativeClassId(String className) {
    // search through all native classes
    for(int i=0;i<nativeClasses.size();i++) {
      NativeClass nativeClass = (NativeClass)nativeClasses.elementAt(i);
      if(nativeClass.className.equals(className))
	return nativeClass.id;
    }
    return -1;
  }
}

