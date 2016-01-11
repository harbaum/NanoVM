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

import java.util.Vector;

/*
 * A class for storing information about the code of a method.
 */
public class CodeInfo {
  private short maxStack, maxLocals;
  private byte[] bytecode;
  private ExceptionInfo[] exceptionTable;
  private LineNumberInfo[] lineNumberTable;
  private LocalVariableInfo[] localVariableTable;
  private Vector attributes = new Vector();
  
  public CodeInfo(short maxStack, short maxLocals, byte[] bytecode, 
		  ExceptionInfo[] exceptionTable) {
      setMaxStack(maxStack);
      setMaxLocals(maxLocals);
      setBytecode(bytecode);
      setExceptionTable(exceptionTable);
  }
  
  public void setMaxStack(short maxStack) {
    this.maxStack = maxStack;
  }
  
  public short getMaxStack() {
    return maxStack;
  }
  
  public void setMaxLocals(short maxLocals) {
    this.maxLocals = maxLocals;
  }
  
  public short getMaxLocals() {
    return maxLocals;
  }
  
  public void setBytecode(byte[] bytecode) {
    this.bytecode = bytecode;
  }
  
  public byte[] getBytecode() {
    return bytecode;
  }
  
  public void setExceptionTable(ExceptionInfo[] exceptionTable) {
    this.exceptionTable = exceptionTable;
  }
  
  public ExceptionInfo[] getExceptionTable() {
    return exceptionTable;
  }
  
  public void setLineNumberTable(LineNumberInfo[] lineNumberTable) {
    this.lineNumberTable = lineNumberTable;
  }
  
  public LineNumberInfo[] getLineNumberTable() {
    return lineNumberTable;
  }
  
  public void setLocalVariableTable(LocalVariableInfo[] localVariableTable) {
    this.localVariableTable = localVariableTable;
  }
  
  public void addAttribute(AttributeInfo attributeInfo) {
    attributes.addElement(attributeInfo);
  }
  
  public AttributeInfo[] getAttributes() {
    AttributeInfo[] list = new AttributeInfo[attributes.size()];
    attributes.copyInto(list);
    return list;
  }
}
