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

/**
 * This class parses java class files and stores the information in a 
 * ClassInfo object.
 *
 * @see ClassInfo
 */
public class ClassFileReader implements AccessFlags {

  public void read(InputStream in, ClassInfo classInfo)
    throws IOException {
    strClass(new DataInputStream(new BufferedInputStream(in)), classInfo);
  }
  
  private void strClass(DataInput in, ClassInfo classInfo)
    throws IOException {
    short count;
    
    // Magic And Version Numbers
    int magic = in.readInt();
    if (magic != 0xCAFEBABE)
      throw new IOException("Invalid classfile magic number" + 
	   ": expected 0xCAFEBABE, found 0x" + Integer.toHexString(magic));

    short major = in.readShort(), minor = in.readShort();
    System.out.println("Classfile version " + major + "." + minor);
    
    // Constant Pool
    ConstPool cp = new ConstPool();
    cp.read(in);
    classInfo.setConstPool(cp);
    
    // General Class Info
    short flags = in.readShort();
    short classIndex = in.readShort();
    short superClassIndex = in.readShort();
    int classNameIndex = cp.getEntryAtIndex(classIndex).getClassNameIndex();
    int superClassNameIndex = cp.getEntryAtIndex(superClassIndex).getClassNameIndex();
    String className = cp.getEntryAtIndex(classNameIndex).getString();
    String superClassName = cp.getEntryAtIndex(superClassNameIndex).getString();
    
    if (Debug.strClass != null) 
      Debug.println(Debug.strClass,
		    "flags=" + flags +
		    "; class index=" + classIndex +
		    "; super class index=" + superClassIndex);
    
    // save necessary class information
    classInfo.setAccessFlags(flags);
    classInfo.setName(className);
    classInfo.setSuperClassName(superClassName);

    // Interfaces
    count = in.readShort();
    if(Debug.strClass != null) 
      Debug.println(Debug.strClass,
		    "#interfaces=" + count);
    Debug.indent();
    for (int i = 0; i < count; i++) 
      classInfo.addInterface(strInterface(cp, in));
    Debug.outdent();
    
    // Fields
    count = in.readShort();
    if (Debug.strClass != null) Debug.println(Debug.strClass,
					       "#fields=" + count);
    Debug.indent();
    for (int i = 0; i < count; i++)
      classInfo.addField(strField(cp, in));
    Debug.outdent();
    
    // Methods
    count = in.readShort();
    if (Debug.strClass != null) 
      Debug.println(Debug.strClass,
		    "#methods=" + count);
    Debug.indent();
    for (int i = 0; i < count; i++)
      classInfo.addMethod(strMethod(cp, in));
    Debug.outdent();
    
    // Attributes
    count = in.readShort();
    if (Debug.strClass != null) 
      Debug.println(Debug.strClass,
		    "#attributes=" + count);
    Debug.indent();
    for (int i = 0; i < count; i++) 
      strClassAttribute(classInfo, cp, in);
    Debug.outdent();
  }
  
  private String strInterface(ConstPool cp, DataInput in)
    throws IOException {
    short classIndex = in.readShort();
    if (Debug.strInterface != null) 
      Debug.println(Debug.strInterface,
		    "class index=" + classIndex);
    int nameIndex = cp.getEntryAtIndex(classIndex).getClassNameIndex();
    return cp.getEntryAtIndex(nameIndex).getString();
  }
  
  private FieldInfo strField(ConstPool cp, DataInput in) 
    throws IOException {
      FieldInfo result;
      
      // General Field Info
      {
	short flags = in.readShort();
	short nameIndex = in.readShort();
	short signatureIndex = in.readShort();
	
	if (Debug.strField != null) Debug.println(Debug.strField,
		   "flags=" + flags +
		   "; name index=" + nameIndex +
		   "; signature index=" + signatureIndex);
	
	result = new FieldInfo(flags, 
			       cp.getEntryAtIndex(nameIndex).getString(), 
			       cp.getEntryAtIndex(signatureIndex).getString());
      }
      
      // Field Attributes
      short numAttributes = in.readShort();
      if (Debug.strField != null) Debug.println(Debug.strField,
			 "#attributes=" + numAttributes);
      Debug.indent();
      for (int i = 0; i < numAttributes; i++)
	strFieldAttribute(result, cp, in);
      Debug.outdent();
      
      return result;
  }
  
  private MethodInfo strMethod(ConstPool cp, DataInput in) 
    throws IOException {
      MethodInfo result;
      
      // General Method Info
      {
	short flags = in.readShort();
	short nameIndex = in.readShort();
	short signatureIndex = in.readShort();
	String methodName = cp.getEntryAtIndex(nameIndex).getString();
	String methodSignature = cp.getEntryAtIndex(signatureIndex).getString();
	
	if (Debug.strMethod != null) Debug.println(Debug.strMethod,
			    "flags=" + flags +
			    "; name index=" + nameIndex +
			    "; signature index=" + signatureIndex);
	
	result = new MethodInfo(flags, methodName, methodSignature);
      }
      
      // Method Attributes
      short methodAttrCount = in.readShort();
      if (Debug.strMethod != null) Debug.println(Debug.strMethod,
			  "#attributes=" + methodAttrCount);
      Debug.indent();
      
      for (int iMethodAttr = 0; iMethodAttr < methodAttrCount; iMethodAttr++)
	strMethodAttribute(result, cp, in);
      
      Debug.outdent();
      
      return result;
  }
  
  private void strClassAttribute(ClassInfo classInfo, ConstPool cp, 
				  DataInput in) throws IOException {
      short nameIndex = in.readShort();
      int length = in.readInt();
      
      // make sure we read the entire attribute -- if it has bad data,
      // an exception might get thrown before we've read it all
      byte[] bytes = new byte[length];
      in.readFully(bytes);
      in = new DataInputStream(new ByteArrayInputStream(bytes));
      
      if (Debug.strClass != null) Debug.println(Debug.strClass,
				 "attribute name index=" + nameIndex +
				 "; length=" + length);
      Debug.indent();
      
      try {
	
	String name = cp.getEntryAtIndex(nameIndex).getString();
	
	// SourceFile Attribute
	if (name.equals("SourceFile")) {
	  short filenameIndex = in.readShort();
	  if (Debug.strClass != null) Debug.println(Debug.strClass,
			     "filename index=" + filenameIndex);
	  classInfo.setSourceFile(cp.getEntryAtIndex(filenameIndex).getString());
	}
	
	else if (name.equals("InnerClasses"))
	  classInfo.setInnerClasses(strInnerClasses(cp, in));
	
	else 
	  classInfo.addAttribute(strUnknownAttribute(name, length, in));
	
      } catch (ConstPoolEntryError e) {
	if (Debug.strBadData != null) Debug.println(Debug.strBadData,
		     "class attribute name index=" + nameIndex);
      }
      
      Debug.outdent();
  }
  
  private void strFieldAttribute(FieldInfo fieldInfo, ConstPool cp, 
				  DataInput in) throws IOException {
      short nameIndex = in.readShort();
      String name = cp.getEntryAtIndex(nameIndex).getString();
      int length = in.readInt();
      
      // make sure we read the entire attribute -- if it has bad data,
      // an exception might get thrown before we've read it all
      byte[] bytes = new byte[length];
      in.readFully(bytes);
      in = new DataInputStream(new ByteArrayInputStream(bytes));
      
      if (Debug.strField != null) Debug.println(Debug.strField,
				 "attribute name index=" + nameIndex +
				 "; length=" + length);
      Debug.indent();
      
      try {
	
	// ConstantValue Attribute
	if (name.equals("ConstantValue")) {
	  short cvIndex = in.readShort();
	  if (Debug.strField != null) Debug.println(Debug.strField, 
			     "constant value index=" + cvIndex);
	  fieldInfo.setConstantValue(cp.getEntryAtIndex(cvIndex).getPrimitiveTypeValue());
	}
	
	else if (name.equals("Synthetic")) {
	  if (Debug.strField != null) Debug.println(Debug.strField,
						     "synthetic");
	  fieldInfo.setSynthetic(true);
	}
	
	else
	  fieldInfo.addAttribute(strUnknownAttribute(name, length, in));
	
      } catch (ConstPoolEntryError e) {
	if (Debug.strBadData != null) Debug.println(Debug.strBadData,
		     "field attribute name index=" + nameIndex);
      }
      
      Debug.outdent();
  }
  
  private void strMethodAttribute(MethodInfo methodInfo, ConstPool cp, 
				   DataInput in) throws IOException {
      short nameIndex = in.readShort();
      int length = in.readInt();
      
      // make sure we read the entire attribute -- if it has bad data,
      // an exception might get thrown before we've read it all
      byte[] bytes = new byte[length];
      in.readFully(bytes);
      in = new DataInputStream(new ByteArrayInputStream(bytes));
      
      if (Debug.strMethod != null) Debug.println(Debug.strMethod,
			  "attribute name index=" + nameIndex +
			  "; length=" + length);
      Debug.indent();
      
      try {
	
	String name = cp.getEntryAtIndex(nameIndex).getString();
	if (name.equals("Exceptions")) {
	  int count = in.readShort();
	  for (int i = 0; i < count; i++) {
	    short exceptionClassIndex = in.readShort();
	    int exceptionClassNameIndex = cp.getEntryAtIndex(exceptionClassIndex).getClassNameIndex();
	    String exceptionName = cp.getEntryAtIndex(exceptionClassNameIndex). getString();
	    methodInfo.addException(exceptionName);
	  }
	}

	else if (name.equals("Code")) 
	  methodInfo.setCodeInfo(strCode(cp, in));

	else if (name.equals("Deprecated")) {
	  if (Debug.strMethod != null) Debug.println(Debug.strMethod,
						      "deprecated");
	  methodInfo.setDeprecated(true);
	}
	
	else 
	  methodInfo.addAttribute(strUnknownAttribute(name, length, in));
	
      } catch (ConstPoolEntryError e) {
	if (Debug.strBadData != null) Debug.println(Debug.strBadData,
		     "method attribute name index=" + nameIndex);
      }
      
      Debug.outdent();
  }
  
  private CodeInfo strCode(ConstPool cp, DataInput in) 
    throws IOException {
    // General Code Info
    short maxStack = in.readShort();
    short maxLocals = in.readShort();
    byte[] bytecode = new byte[in.readInt()];

    if (Debug.strCode != null) Debug.println(Debug.strCode,
		      "maxStack=" + maxStack +
		      "; maxLocals=" + maxLocals +
		      "; bytecode length=" + bytecode.length);
    
    in.readFully(bytecode);
    
    // Exception Table
    ExceptionInfo[] exceptionTable = new ExceptionInfo[in.readShort()];
    if (Debug.strCode != null) Debug.println(Debug.strCode,
		  "exception table length=" + exceptionTable.length);
    Debug.indent();
    for (int i = 0; i < exceptionTable.length; i++) {
      short startPC = in.readShort();
      short endPC = in.readShort();
      short handlerPC = in.readShort();
      short catchTypeIndex = in.readShort();
      
      if (Debug.strCode != null) Debug.println(Debug.strCode,
		"startPC=" + startPC +
		"; endPC=" + endPC +
		"; handlerPC=" + handlerPC +
		"; catchTypeIndex=" + catchTypeIndex);

      String catchType = null;
      if (catchTypeIndex != 0) {	// index is null for finally blocks
	int catchTypeNameIndex = 
	  cp.getEntryAtIndex(catchTypeIndex).getClassNameIndex();
	catchType = cp.getEntryAtIndex(catchTypeNameIndex).getString();
      }
      exceptionTable[i] = 
	new ExceptionInfo(startPC, endPC, handlerPC, catchType);
    }
    Debug.outdent();
    
    CodeInfo codeInfo = 
      new CodeInfo(maxStack, maxLocals, bytecode, exceptionTable);
    
    // Code Attributes
    short codeAttrCount = in.readShort();
    if (Debug.strCode != null) Debug.println(Debug.strCode,
			      "#attributes=" + codeAttrCount);
    Debug.indent();
    for (int iCodeAttr = 0; iCodeAttr < codeAttrCount; iCodeAttr++)
      strCodeAttribute(codeInfo, cp, in);
    Debug.outdent();
    
    return codeInfo;
  }		
  
  private void strCodeAttribute(CodeInfo codeInfo, ConstPool cp, 
				 DataInput in) throws IOException {
    short nameIndex = in.readShort();
    int length = in.readInt();
    
    // make sure we read the entire attribute -- if it has bad data,
    // an exception might get thrown before we've read it all
    byte[] bytes = new byte[length];
    in.readFully(bytes);
    in = new DataInputStream(new ByteArrayInputStream(bytes));
    
    if (Debug.strCode != null) 
      Debug.println(Debug.strCode, "code attribute name index=" + 
		    nameIndex + "; length=" + length);
    Debug.indent();
    
    try {
      
      String name = cp.getEntryAtIndex(nameIndex).getString();
      if (name.equals("LineNumberTable")) 
	codeInfo.setLineNumberTable(readLineNumberTable(in));
      else if (name.equals("LocalVariableTable")) 
	codeInfo.setLocalVariableTable(strLocalVariablesTable(cp, in));
      else
	codeInfo.addAttribute(strUnknownAttribute(name, length, in));
      
    } catch (ConstPoolEntryError e) {
      if (Debug.strBadData != null) Debug.println(Debug.strBadData,
			   "code attribute name index=" + nameIndex);
    }
    
    Debug.outdent();
  }
  
  private InnerClassInfo[] strInnerClasses(ConstPool cp, DataInput in) 
    throws IOException {
    short rows = in.readShort();
    if (Debug.strInnerClasses != null) Debug.println(Debug.strInnerClasses,
				      "#inner classes=" + rows);
    InnerClassInfo[] classes = new InnerClassInfo[rows];
    for (int i = 0; i < rows; i++) {
      short innerClassIndex = in.readShort();
      short outerClassIndex = in.readShort();
      short simpleNameIndex = in.readShort();
      short flags = in.readShort();
      
      if (Debug.strInnerClasses != null) 
	Debug.println(Debug.strInnerClasses,
		      "inner class index=" + innerClassIndex +
		      "; outer class index=" + outerClassIndex +
		      "; simple name index=" + simpleNameIndex +
		      "; flags=" + flags);
      
      int innerClassNameIndex = cp.getEntryAtIndex(innerClassIndex).getClassNameIndex();
      String innerClassName = cp.getEntryAtIndex(innerClassNameIndex).getString();
      String outerClassName = null;
      if (outerClassIndex != 0) {
	int outerClassNameIndex = cp.getEntryAtIndex(outerClassIndex).getClassNameIndex();
	outerClassName = cp.getEntryAtIndex(outerClassNameIndex).getString();
      }
      String simpleName = null;
      if (simpleNameIndex != 0)
	simpleName = cp.getEntryAtIndex(simpleNameIndex).getString();
      
      classes[i] = new InnerClassInfo(innerClassName, outerClassName, simpleName, flags);
    }
    return classes;
  }
  
  private LocalVariableInfo[] strLocalVariablesTable(ConstPool cp, DataInput in) 
    throws IOException
    {
      LocalVariableInfo[] table = new LocalVariableInfo[in.readShort()];
      if (Debug.strLocalVariables != null) 
	Debug.println(Debug.strLocalVariables,
		      "#local variables=" + table.length);
      for (int i = 0; i < table.length; i++) {
	short startPC = in.readShort();
	short length = in.readShort();
	short nameIndex = in.readShort();
	short signatureIndex = in.readShort();
	short slot= in.readShort();
	if (Debug.strLocalVariables != null) 
	  Debug.println(Debug.strLocalVariables,
			"start PC=" + startPC +
			"; length=" + length +
			"; name index=" + nameIndex +
			"; signature index=" + signatureIndex +
			"; slot=" + slot);
	
	String name = cp.getEntryAtIndex(nameIndex).getString();
	String signature = cp.getEntryAtIndex(signatureIndex).getString();
	table[i] = new LocalVariableInfo(startPC, length, name, signature, slot);
      }
      return table;
    }
  
  private LineNumberInfo[] readLineNumberTable(DataInput in)
    throws IOException {
    LineNumberInfo[] table = new LineNumberInfo[in.readShort()];
    if (Debug.strLineNumbers != null) 
      Debug.println(Debug.strLineNumbers,
		    "#line numbers=" + table.length);
    for (int i = 0; i < table.length; i++) {
      short startPC = in.readShort();
      short lineNumber = in.readShort();
      
      if (Debug.strLineNumbers != null) 
	Debug.println(Debug.strLineNumbers,
		      "start PC=" + startPC +
		      "; line number=" + lineNumber);
      
      table[i] = new LineNumberInfo(startPC, lineNumber);
    }
    return table;
  }
  
  private AttributeInfo strUnknownAttribute(String name, int length, DataInput in) 
    throws IOException {
    if (Debug.strUnknownAttribute != null) 
      Debug.println(Debug.strUnknownAttribute,
		    "attribute name=\"" + name + "\"");
    byte[] data = new byte[length];
    in.readFully(data);
    return new AttributeInfo(name, data);
  }
}
