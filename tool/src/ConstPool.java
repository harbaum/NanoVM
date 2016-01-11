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

import java.util.*;
import java.io.*;

/**
* This class encapsulates constant pool management.  Classfiles use
* constant pools to store information such as class names, method 
* names, type signatures, and literals like "Hello World" or 123. When 
* the information is needed (like for a method declaration), an index 
* into the constant pool is given instead of the actual information.  
* This can reduce the size of a classfile dramatically if the same 
* information is used frequently.
*
* <p>ClassFileReaders and ClassFileWriters usually try to convert
* constant pool indexes into meaningful data (eg ClassInfo.setName() 
* takes a String instead of a constant pool index).  So you probably 
* won't need to use this class unless you need to work with attributes 
* that the ClassFileReader/Writer doesn't understand, such as 
* bytecodes.
*
* <p>The constant pool is a 0-based array of entries.  The entry at
* index 0 is never used.  That is,
* 
* <pre>constPool.getEntryAtIndex(0).isUnused() == true;</pre>
* 
* Another quirk of the constant pool is that LONG and DOUBLE
* entries take up "two" indexes.  That is,
*
* <pre>short longIndex = constPool.addEntry(new ConstPoolEntry().setLong(0));
* constPool.getEntryAtIndex(longIndex+1).isUnused() == true;</pre>
*
* @see ConstPoolEntry
* @see Signature
*/
public class ConstPool {
  private Vector cp = new Vector();
  // for efficiency, addEntry() caches elements in a hashtable, which is
  // used by getEntryAtIndex().  Caching reduces the time required to 
  // read ClassFileWriter.class from ~10s to ~7.5s.
  private Hashtable cpHash = new Hashtable();
  
  /**
   * Get the size of the pool.
   */
  public int size() {
    return cp.size();
  }
  
  /**
   * Add a constant pool entry, whether or not it's already in the pool.
   * If the entry is a LONG or DOUBLE, a second, unused entry is 
   * also added.
   * @returns the index of the added entry.
   */
  public int addEntry(ConstPoolEntry entry) {
    cp.addElement(entry);
    int result = size() - 1;
    cpHash.put(entry, new Integer(result));
    // LONG and DOUBLE entries take up two entries
    if (entry.typecode() == ConstPoolEntry.LONG 
	|| entry.typecode() == ConstPoolEntry.DOUBLE) 
      cp.addElement(new ConstPoolEntry().setUnused());
    return result;
  }

  /**
   * Get the const pool entry at a given index.
   */
  public ConstPoolEntry getEntryAtIndex(int index) {
    return (ConstPoolEntry)cp.elementAt(index);
  }
  
  /**
   * Return the index of a constant pool entry equal to the specified one.  
   * If there is no matching entry in the pool yet, -1 is returned.  Comparisons 
   * are by value, not by reference.
   */
  public int getIndexOfEntryNoAdd(ConstPoolEntry entry) {
    Integer i = (Integer)cpHash.get(entry);
    if (i != null)
      return i.intValue();
    return -1;
  }
  
  /**
   * Return the index of a constant pool entry equal to the specified one.  
   * If there is no matching entry in the pool yet, a clone of the specified 
   * entry is added.  Comparisons are made by value, not by reference.
   */
  public int getIndexOfEntryAdd(ConstPoolEntry entry) {
    int result = getIndexOfEntryNoAdd(entry);
    if (result < 0)
      result = addEntry((ConstPoolEntry)entry.clone());
    return result;
  }
  
  // used for doing searches -- reuse so we don't have to allocate a new
  // one every time.
  private ConstPoolEntry searchEntry = new ConstPoolEntry();
  
  /**
   * Convenience method to return the index of the UTF const pool entry 
   * equal to the given string.  If there is no matching entry in the const pool yet, 
   * a new one is added.  Without this method, something like the following code 
   * would be used:
   * <pre>constPool.getIndexOfEntryAdd(new ConstPoolEntry().setUTF(str));</pre>
   * This method avoids the overhead of allocating a ConstPoolEntry every time.
   */
  public int getIndexOfUTFAdd(String str) {
    return getIndexOfEntryAdd(searchEntry.setUTF(str));
  }
  
  /**
   * Convenience method to return the index of the CLASS const pool entry 
   * whose classname is equal to the given string.  If there is no matching entry in 
   * the const pool yet, a new one is added.  Without this method, something like 
   * the following code would be used:
   * <pre>short nameIndex = constPool.getIndexOfUTFAdd(classname);
   * constPool.getIndexEntryAdd(new ConstPoolEntry().setClass(nameIndex));</pre>
   * This method avoids the overhead of allocating a ConstPoolEntry every time.
   */
  public int getIndexOfClassAdd(String classname) {
    searchEntry.setClass(getIndexOfUTFAdd(classname));
    return getIndexOfEntryAdd(searchEntry);
  }
  
  /**
   * Read in the constant pool from a stream.
   * @throws ClassFileReadException if the class file is corrupt.
   * @throws IOException if the DataInput throws an IOException.
   */
  public void read(DataInput in) throws IOException {
    int count = in.readShort();
    
    if (Debug.strConstPool != null) 
      Debug.println(Debug.strConstPool, 
		    "# of entries = " + count);
    
    // entry 0 is unused
    addEntry(new ConstPoolEntry().setUnused());
    // note: start i at 1 because 0 is unused
    for (int i = 1; i < count; i++) {
      ConstPoolEntry entry = new ConstPoolEntry();
      entry.read(in, entry);
      
      if (Debug.strConstPool != null) 
	Debug.println(Debug.strConstPool, 
		      i + " " + entry.toString());
      
      addEntry(entry);
      // LONG and DOUBLE entries take up two slots
      if (ConstPoolEntry.LONG == entry.typecode() 
	  || ConstPoolEntry.DOUBLE == entry.typecode())
	i++;
    }
  }

  // return total number of constant entries
  public int totalConstantEntries() {
    int num=0;

    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);
      
      // check for ints and floats only
      if((entry.typecode() == ConstPoolEntry.FLOAT)
        ||(entry.typecode() == ConstPoolEntry.INT))
           num++;
    }
    return num;
  }

  // return the n'th constant entry in this constant pool
  public int getConstantEntry(int n) {
    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);
      
      // check for ints and floats only
      if(entry.typecode() == ConstPoolEntry.FLOAT)
      {  
        if(n-- == 0)
          return encodeFloat(entry.getFloat());
      }
      
      if(entry.typecode() == ConstPoolEntry.INT)
      {  
        if(n-- == 0)
          return entry.getInt();
      }
    }
    return 0;
  }


  // return total number of strings
  public int totalStrings() {
    int num=0;

    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);
      
      // check for strings only
      if(entry.typecode() == ConstPoolEntry.STRING) 
	num++;
    }
    return num;
  }

  // return size of all strings stored in this constant pool
  public int totalStringSize() {
    int sum=0;

    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);

      // check for strings only
      if(entry.typecode() == ConstPoolEntry.STRING) {
	// get everything we need to know about the methods
	ConstPoolEntry stringRef = 
	  getEntryAtIndex(entry.getStringIndex());

	sum += stringRef.getString().length()+1;
      }
    }
    return sum;
  }

  // return the n'th string in this constant pool
  public String getString(int n) {
    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);
      
      // check for strings only
      if(entry.typecode() == ConstPoolEntry.STRING) 
	if(n-- == 0) 
	  return getEntryAtIndex(entry.getStringIndex()).getString();
    }
    return null;
  }

  String getClassName(ConstPoolEntry entry) {
    return getEntryAtIndex(getEntryAtIndex(entry.getClassIndex()).
			   getClassNameIndex()).getString();
  }

  String getMethodName(ConstPoolEntry entry) {
    return getEntryAtIndex(getEntryAtIndex(entry.getNameAndTypeIndex()).
			   getNameIndex()).getString();
  }

  String getMethodType(ConstPoolEntry entry) {
    return getEntryAtIndex(getEntryAtIndex(entry.getNameAndTypeIndex()).
			   getTypeIndex()).getString();
  }

  String getFieldName(ConstPoolEntry entry) {
    return getEntryAtIndex(getEntryAtIndex(entry.getNameAndTypeIndex()).
			   getNameIndex()).getString();
  }

  String getFieldType(ConstPoolEntry entry) {
    return getEntryAtIndex(getEntryAtIndex(entry.getNameAndTypeIndex()).
			   getTypeIndex()).getString();
  }

  // since our final file does not contain a constant pool, the various
  // references into the constant pool has to ne updated into something 
  // that can be directly used to call methods, push string references etc.
  // This is what the following code does with an index to a constant pool
  // reference.

  static int encodeFloat(float val) {
    int ival = Float.floatToRawIntBits(val);
    boolean sign = ival<0;
    int exponent = ((ival>>23)&0xff);
    ival &= 0x007fffff;

    if (exponent==0xff)
      exponent=0x7f;
    else if (exponent!=0x00)
      exponent-=0x40;
      
    ival |= (exponent<<23);
    if (sign)
      ival |= 0x40000000;

    ival &= 0x7fffffff;
    return ival;
  }

  static int encodeInt(int val) {
    val = val & 0x7fffffff;
    return val;
  }

  
  // referenz auf diese klasse mitgeben
  public int constantRelocate(int index) {
    ConstPoolEntry entry = getEntryAtIndex(index);
    int id = 0xffff;
    
    System.out.print(" -> ");

    if(entry.typecode() == ConstPoolEntry.CLASS) {
      String className = getEntryAtIndex(
	entry.getClassNameIndex()).getString();

      System.out.print("class, " + className);
      
      // check if there's a local class with this name
      id = ClassLoader.getClassIndex(className);
      if(id >= 0) 
	System.out.println("local id: #" + Integer.toHexString(id<<8));
      else {
	id = NativeMapper.getNativeClassId(className);
	if(id >= 0) 
	  System.out.println("native id: #" + Integer.toHexString(id<<8));
	else {
	  System.out.println("Unable to map class reference");
	  System.exit(-1);
	}
      }
      id <<= 8; // class id
    }

    else if(entry.typecode() == ConstPoolEntry.FIELDREF) {
      System.out.print("field " + getClassName(entry) + ":" + 
		       getFieldName(entry) +"."+ getFieldType(entry) + ", ");

      // check if this is a local field
      // try to get the local method index
      if(ClassLoader.fieldExists(
	   getClassName(entry), getMethodName(entry), getMethodType(entry))) {
	System.out.print("local, ");

	// check if static or not
	FieldInfo fieldInfo = ClassLoader.getFieldInfo(
	    getClassName(entry), getMethodName(entry), getMethodType(entry));
	
	if((fieldInfo.getAccessFlags() & AccessFlags.STATIC) != 0) {
	  // index is the number of field in this whole set of classes
	  id = ClassLoader.getStaticFieldIndex(getClassName(entry), 
				 getMethodName(entry), getMethodType(entry));

	  System.out.println("static id: #" + Integer.toHexString(id));
	} else {
	  // non-static, get access to class it resides in
	  ClassInfo classInfo = ClassLoader.getClassInfo(getClassName(entry));

	  // index is the number of non static fields in this local class
	  id = classInfo.getFieldIndex(0, getMethodName(entry), getMethodType(entry));

	  // plus the total number of fields in the super classes	  
	  // walk through all super classes
	  String className = getClassName(entry);	  
	  while(NativeMapper.getNativeClassId(ClassLoader.
			 getSuperClassName(className)) == -1) {
	    className = ClassLoader.getSuperClassName(className);
	    id += ClassLoader.getClassInfo(className).nonStaticFields();
	  }
	  System.out.println("non static id: #" + Integer.toHexString(id));
	}

      } else {
	// we only support static native fields
	id = NativeMapper.getFieldId(
	  getClassName(entry), getMethodName(entry), getMethodType(entry));
	
	if(id == -1) {
	  System.out.println("Unable to map field reference");
          System.out.println("class="+getClassName(entry)+" method="+getMethodName(entry)+" type="+getMethodType(entry));
	  System.exit(-1);
	}

	System.out.println("native, id: #" + Integer.toHexString(id));
      }
    }

    else if(entry.typecode() == ConstPoolEntry.STRING) {
      String str = getEntryAtIndex(entry.getStringIndex()).getString();
      System.out.print("string \"" + str + "\", ");

      // search for this string
      for(int i=0;i<ClassLoader.totalStrings();i++) 
	if(ClassLoader.getString(i).equals(str))
	  id = i + ClassLoader.totalConstantEntries();

      if(id == 0xffff) {
	System.out.println("Unable to map string reference");
	System.exit(-1);	  
      } else
	System.out.println("id: #" + Integer.toHexString(id));
    }
    
    else if(entry.typecode() == ConstPoolEntry.METHODREF) {

      System.out.print("method " + getClassName(entry) + ":" + 
		       getMethodName(entry) +"."+ getMethodType(entry) + ", ");

      // try to get the local method index
      index = ClassLoader.getMethodIndex(
	   getClassName(entry), getMethodName(entry), getMethodType(entry));

      // didn't work? try native methods
      if(index == -1) {
	id = NativeMapper.getMethodId(
	  getClassName(entry), getMethodName(entry), getMethodType(entry));

	if(id == -1) {
	  System.out.println("Unable to map method reference");
	  System.exit(-1);	  
	}

	System.out.println("native id: #" + Integer.toHexString(id));
      } else {
	// new id is just the index of the method in the file
	id = index;
	System.out.println("local id: #" + Integer.toHexString(id));
      }
    }

    else if(entry.typecode() == ConstPoolEntry.FLOAT) {
      float val = entry.getFloat();
      int ival = encodeFloat(val);

      System.out.print("float :"+ val + " = 0x" + Integer.toHexString(ival) + ", ");

      // search for this constant
      for(int i=0;i<ClassLoader.totalConstantEntries();i++) 
        if(ClassLoader.getConstantEntry(i)==ival)
          id = i;

      if(id == 0xffff) {
        System.out.println("Unable to map constant");
        System.exit(-1);          
      } else
        System.out.println("id: #" + Integer.toHexString(id));
    }

    else if(entry.typecode() == ConstPoolEntry.INT) {
      int val = entry.getInt();
      int ival = encodeInt(val);

      System.out.print("integer :"+ val + " = 0x" + Integer.toHexString(ival) + ", ");

      // search for this constant
      for(int i=0;i<ClassLoader.totalConstantEntries();i++) 
        if(ClassLoader.getConstantEntry(i)==ival)
          id = i;

      if(id == 0xffff) {
        System.out.println("Unable to map constant");
        System.exit(-1);          
      } else
        System.out.println("id: #" + Integer.toHexString(id));
    }

    else {
      System.out.println("ERROR: Not a relocatable type");      
      System.exit(-1);
    }

    return id;
  }
    
  public void resolveMethodRefs() {
    System.out.println("Resolving method references ...");

    for(int i=0;i<size();i++) {
      ConstPoolEntry entry = getEntryAtIndex(i);

      // check for method references only
      if(entry.typecode() == ConstPoolEntry.METHODREF) {
	// get everything we need to know about the methods
	System.out.print("Method " + getClassName(entry) +"."+ 
			 getMethodName(entry) +":"+ getMethodType(entry));

	if(!NativeMapper.methodIsNative(getClassName(entry), 
				getMethodName(entry), getMethodType(entry))) {

	  // check if we already know a method like this
	  if(!ClassLoader.methodExists(getClassName(entry), 
		       getMethodName(entry), getMethodType(entry))) {
	     System.out.println(" -> missing");

	    // try to load appropriate class
	    ClassLoader.load(getClassName(entry));
	  } else
	    System.out.println(" -> already present");
	} else
	  System.out.println(" -> native");
      }
    }
  }
}
