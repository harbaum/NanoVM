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

public final class ConstPoolEntry implements Cloneable {
  public static final int UTF = 1;
  public static final int UNICODE = 2;
  public static final int INT = 3;
  public static final int FLOAT = 4;
  public static final int LONG = 5;
  public static final int DOUBLE = 6;
  public static final int CLASS = 7;
  public static final int STRING = 8;
  public static final int FIELDREF = 9;
  public static final int METHODREF = 10;
  public static final int INTERFACEMETHODREF = 11;
  public static final int NAMEANDTYPE = 12;
  
  private byte typecode = -1;
  
  private String stringValue;
  private int intValue;
  private long longValue;
  private float floatValue;
  private double doubleValue;
  private int index1, index2;
  
  public boolean equals(Object o) {
    ConstPoolEntry other;
    try {
      other = (ConstPoolEntry)o;
    } catch (ClassCastException e) {
      return false;
    }

    // For equality purposes, UTF and UNICODE typecodes are equivalent.
    // We special case UTF and UNICODE comparison by ignoring typecode and
    // just looking at stringValue.
    if (stringValue != null && other.stringValue != null)
      return stringValue.equals(other.stringValue);
    
    if (typecode != other.typecode)
      return false;
    
    // This relies on fact that all unused fields are 0.
    return (other.intValue == intValue && 
	    other.longValue == longValue &&
	    other.index1 == index1 &&
	    other.index2 == index2 &&
	    other.floatValue == floatValue &&
	    other.doubleValue == doubleValue);
  }

  public int hashCode() {
    int result = typecode * 97;
    if (stringValue != null)
      result += stringValue.hashCode()*43;
    result += intValue*37 + longValue*31 + index1*29 + index2*23;
    return result;
  }

  public Object clone() {
    ConstPoolEntry result = new ConstPoolEntry();
    result.typecode = typecode;
    result.stringValue = stringValue;
    result.intValue = intValue;
    result.longValue = longValue;
    result.index1 = index1;
    result.index2 = index2;
    result.floatValue = floatValue;
    result.doubleValue = doubleValue;
    return result;
  }

  public String toString() {
    if (UTF == typecode)
      return "UTF \"" + stringValue + "\"";
    else if (UNICODE == typecode)
      return "UNICODE \"" + stringValue + "\"";
    else if (INT == typecode)
      return "INT " + intValue;
    else if (FLOAT == typecode)
      return "FLOAT " + floatValue;
    else if (LONG == typecode)
      return "LONG " + longValue;
    else if (DOUBLE == typecode)
      return "DOUBLE " + doubleValue;
    else if (CLASS == typecode)
      return "CLASS " + index1;
    else if (STRING == typecode)
      return "STRING " + index1;
    else if (FIELDREF == typecode)
      return "FIELDREF " + index1 + " " + index2;
    else if (METHODREF == typecode)
      return "METHODREF " + index1 + " " + index2;
    else if (INTERFACEMETHODREF == typecode)
      return "INTERFACEMETHODREF " + index1 + " " + index2;
    else if (NAMEANDTYPE == typecode)
      return "NAMEANDTYPE " + index1 + " " + index2;
    else
      return "Unknown typecode " + typecode;
  }
  
  public byte typecode() {
    return typecode;
  }  
  
  // used by set methods to erase evidence of any previous set method
  private void clearType() {
    typecode = -1;
    stringValue = null;
    index1 = 0;
    index2 = 0;
    intValue = 0;
    longValue = 0;
    floatValue = 0;
    doubleValue = 0;
  }
  /**
   * Some const pool entries don't actually exist -- like the ones after a 
   * LONG or DOUBLE, or entry 0.  These entries are unused.
   */
  public ConstPoolEntry setUnused() {
    typecode = -1;
    return this;
  }

  public boolean isUnused() {
    return (typecode == -1);
  }

  public ConstPoolEntry setUTF(String value) {
    clearType();
    typecode = UTF;
    stringValue = value;
    if (null == stringValue)
      throw new ConstPoolEntryError("ConstPoolEntry.setUTF called with null string");
    return this;
  }

  public ConstPoolEntry setUnicode(String value) {
    clearType();
    typecode = UNICODE;
    stringValue = value;
    if (null == stringValue)
      throw new ConstPoolEntryError("ConstPoolEntry.setUTF called with null string");
    return this;
  }

  public ConstPoolEntry setInt(int value) {
    clearType();
    typecode = INT;
    intValue = value;
    return this;
  }

  public ConstPoolEntry setLong(long value) {
    clearType();
    typecode = LONG;
    longValue = value;
    return this;
  }
  
  public ConstPoolEntry setFloat(float value) {
    clearType();
    typecode = FLOAT;
    floatValue = value;
    return this;
  }

  public ConstPoolEntry setDouble(double value) {
    clearType();
    typecode = DOUBLE;
    doubleValue = value;
    return this;
  }

  public ConstPoolEntry setString(int index) {
    clearType();
    typecode = STRING;
    index1 = index;
    return this;
  }

  public ConstPoolEntry setClass(int index) {
    clearType();
    typecode = CLASS;
    index1 = index;
    return this;
  }

  public ConstPoolEntry setFieldRef(int index1, int index2) {
    clearType();
    typecode = FIELDREF;
    this.index1 = index1;
    this.index2 = index2;
    return this;
  }
	
  public ConstPoolEntry setMethodRef(int index1, int index2) {
    clearType();
    typecode = METHODREF;
    this.index1 = index1;
    this.index2 = index2;
    return this;
  }

  public ConstPoolEntry setInterfaceMethodRef(int index1, int index2) {
    clearType();
    typecode = INTERFACEMETHODREF;
    this.index1 = index1;
    this.index2 = index2;
    return this;
  }

  public ConstPoolEntry setNameAndType(int index1, int index2) {
    clearType();
    typecode = NAMEANDTYPE;
    this.index1 = index1;
    this.index2 = index2;
    return this;
  }
  
  
  /**
   * For UTF and UNICODE entries.
   */
  public String getString() {
    if (UTF == typecode || UNICODE == typecode)
      return stringValue;
    throw getError("getString()");
  }
  
  /**
   * For STRING entries.
   */
  public int getStringIndex() {
    if (STRING == typecode)
      return index1;
    throw getError("getStringIndex()");
  }

  /**
   * For INT entries.
   */
  public int getInt() {
    if (INT == typecode)
      return intValue;
    throw getError("getInt()");
  }

  /**
   * For LONG entries.
   */
  public long getLong() {
    if (LONG == typecode)
      return longValue;
    throw getError("getLong()");
  }

  /**
   * For FLOAT entries.
   */
  public float getFloat() {
    if (FLOAT == typecode)
      return floatValue;
    throw getError("getFloat()");
  }

  /**
   * For DOUBLE entries.
   */
  public double getDouble() {
    if (DOUBLE == typecode)
      return doubleValue;
    throw getError("getDouble()");
  }

  /**
   * For INT, LONG, FLOAT, DOUBLE, UTF, or UNICODE entries,
   * returns an Integer, Long, Float, Double, or String, respectively.
   */
  public Object getPrimitiveTypeValue() {
    if (UTF == typecode || UNICODE == typecode) return stringValue;
    if (INT == typecode)                        return new Integer(intValue);
    if (LONG == typecode)                       return new Long(longValue);
    if (FLOAT == typecode)                      return new Float(floatValue);
    if (DOUBLE == typecode)                     return new Double(doubleValue);
    throw getError("getPrimitiveTypeValue()");
  }

  /**
   * For CLASS entries.
   */
  public int getClassNameIndex() {
    if (CLASS == typecode)
      return index1;
    throw getError("getClassNameIndex()");
  }

  /**
   * For FIELDREF, METHODREF, or INTERFACEMETHODREF entries.
   */
  public int getClassIndex() {
    if (FIELDREF == typecode || METHODREF == typecode 
	|| INTERFACEMETHODREF == typecode)
      return index1;
    throw getError("getClassIndex()");
  }

  /**
   * For FIELDREF, METHODREF, or INTERFACEMETHODREF entries.
   */
  public int getNameAndTypeIndex() {
    if (FIELDREF == typecode || METHODREF == typecode
	|| INTERFACEMETHODREF == typecode)
      return index2;
    throw getError("getNameAndTypeIndex()");
  }

  /**
   * For NAMEANDTYPE entries.
   */
  public int getNameIndex() {
    if (NAMEANDTYPE == typecode)
      return index1;
    throw getError("getNameIndex()");
  }

  /**
   * For NAMEANDTYPE entries.
   */
  public int getTypeIndex() {
    if (NAMEANDTYPE == typecode)
      return index2;
    throw getError("getTypeIndex()");
  }
  
  // utility method -- called when getXXXX() is 
  // called for inappropriate typecode
  private ConstPoolEntryError getError(String method) {
    return new ConstPoolEntryError(method + 
	   " called on ConstPoolEntry with typecode " + 
	   typecode());
  }
  
  public void read(DataInput in, ConstPoolEntry entry) 
    throws IOException {
    byte tag = in.readByte();
    switch (tag) {
      case UTF:
	entry.setUTF(in.readUTF());
	break;
      case UNICODE:
	int length = in.readShort();
	char[] chars = new char[length];
	for (int i = 0; i < length; i++)
	  chars[i] = in.readChar();
	String str = new String(chars);
	entry.setUnicode(str);
	break;
      case INT:
	entry.setInt(in.readInt());
	break;
      case FLOAT:
	entry.setFloat(in.readFloat());
	break;
      case LONG:
	entry.setLong(in.readLong());
	break;
      case DOUBLE:
	entry.setDouble(in.readDouble());
	break;
      case CLASS:
	entry.setClass(in.readShort());
	break;
      case STRING:
	entry.setString(in.readShort());
	break;
      case FIELDREF:
	entry.setFieldRef(in.readShort(), in.readShort());
	break;
      case METHODREF:
	entry.setMethodRef(in.readShort(), in.readShort());
	break;
      case INTERFACEMETHODREF:
	entry.setInterfaceMethodRef(in.readShort(), in.readShort());
	break;
      case NAMEANDTYPE:
	entry.setNameAndType(in.readShort(), in.readShort());
	break;
      default:
	throw new IOException("Unknown constant pool tag: " + tag);
    }
  }
}
