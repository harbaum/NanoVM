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

/**
* A class for storing information about a class.  
* This information can be read in by a ClassFileReader, or written out by
* a ClassFileWriter.
*
* @see FieldInfo
* @see MethodInfo
* @see AttributeInfo
* @see ClassFileReader
* @see ClassFileWriter
*/
public class ClassInfo extends CommonInfo {
  private String superName = "java.lang.Object", sourceFile;
  private ConstPool cp;
  // Vector of Strings
  private Vector interfaces = new Vector();
  // Vector of FieldInfo
  private Vector fields = new Vector();
  // Vector of MethodInfo
  private Vector methods = new Vector();
  private InnerClassInfo[] innerClasses;
  
  /**
   * Set the constant pool.  The ClassInfo class itself does not actually use 
   * the ConstPool for anything. This method is useful if the ClassInfo is 
   * being read in by a ClassFileReader and there are attributes that the 
   * Reader does not understand, which reference entries in the constant 
   * pool.  The best example of such an attribute is method bytecodes.
   * In a future release, ClassFileReaders will understand bytecodes, 
   * and so this method will become less important.  
   *
   * <p>The new ConstPool is not immutable.  It may be passed to a 
   * ClassFileWriter, which will add entries as needed.
   *
   * @see ConstPool
   */
  public void setConstPool(ConstPool cp) {
    this.cp = cp;
  }
  
  /**
   * Get the constant pool.  The ClassInfo class itself does not use the
   * constant pool for anthing. The constant pool is useful if the ClassInfo is 
   * being written out by a ClassFileWriter and there are attributes that the 
   * Writer does not understand, which reference entries in the constant 
   * pool.  The best example of such an attribute is method bytecodes.
   * The Writer will use the ConstPool returned by this method in preference
   * to creating its own ConstPool.  Letting the Writer create its own 
   * ConstPool is probably not a good idea, for the reasons describe above.
   * In a future release, ClassFileWriters will understand bytecodes, and so
   * this method will become less important.
   *
   * @see ConstPool
   */
  public ConstPool getConstPool() {
    return cp;
  }
  
  /**
   * Set the name of the super class.  The name should be of the form "java.lang.String".
   */
  public void setSuperClassName(String name) {
    superName = name;
  }
  
  /**
   * Get the name of the super class.  The name is of the form "java.lang.String".
   */
  public String getSuperClassName() {
    return superName;
  }
  
  /**
   * Add an interface that the class implements.
   */
  public void addInterface(String interfaceName) {
    interfaces.addElement(interfaceName);
  }
  
  /**
   * Get the interfaces that this class implements.
   */
  public String[] getInterfaces() {
    String[] list = new String[interfaces.size()];
    interfaces.copyInto(list);
    return list;
  }
  
  /**
   * Add a field to this class.  The FieldInfo should be need not be
   * entirely filled out before being added.
   * 
   * @see FieldInfo
   */
  public void addField(FieldInfo fieldInfo) {
    fields.addElement(fieldInfo);
  }
  
  // get field number n
  public FieldInfo getField(int index) {
    return (FieldInfo)fields.elementAt(index);
  }

  /**
   */
  public void addMethod(MethodInfo methodInfo) {
    methods.addElement(methodInfo);
  }
  
  public MethodInfo getMethod(int index) {
    return (MethodInfo)methods.elementAt(index);
  }

  // return index of given method within current class
  public int getMethodIndex(MethodInfo methodInfo) {
    for(int i=0;i<methods.size();i++) 
      if(getMethod(i) == methodInfo)
	return i;
    
    return -1;
  }

  // get index of method with specified name/type
  public int getMethodIndex(String name, String type) {
    for(int i=0;i<methods.size();i++) {
      MethodInfo methodInfo = (MethodInfo)methods.elementAt(i);

      if(methodInfo.getName().equals(name)) {
	if(type == null) return i;
	
	if(methodInfo.getSignature().equals(type)) 
	  return i;
      }
    }
    return -1;
  }

  public int methods() {
    return methods.size();
  }

  public boolean providesMethod(String name, String type) {
    for(int i=0;i<methods.size();i++) {
      MethodInfo methodInfo = (MethodInfo)methods.elementAt(i);

      if(methodInfo.getName().equals(name)) {
	if(type == null) return true;
	
	if(methodInfo.getSignature().equals(type)) 
	  return true;
      }
    }
    return false;
  }

  public boolean providesField(String name, String type) {
    for(int i=0;i<fields.size();i++) {
      FieldInfo fieldInfo = (FieldInfo)fields.elementAt(i);

      if(fieldInfo.getName().equals(name)) {
	if(type == null) return true;
	
	if(fieldInfo.getSignature().equals(type)) 
	  return true;
      }
    }
    return false;
  }

  public FieldInfo getFieldInfo(String name, String type) {
    for(int i=0;i<fields.size();i++) {
      FieldInfo fieldInfo = (FieldInfo)fields.elementAt(i);

      if(fieldInfo.getName().equals(name)) {
	if(type == null) return fieldInfo;
	
	if(fieldInfo.getSignature().equals(type)) 
	  return fieldInfo;
      }
    }
    return null;
  }

  // return number of static fields of this class
  public int staticFields() {
    int sum = 0;
    
    for(int i=0;i<fields.size();i++) 
      if((getField(i).getAccessFlags() & AccessFlags.STATIC) != 0)
	sum++;

    return sum;
  }

  public int getFieldIndex(int staticFlag, String name, String type) {
    int cnt = 0;

    for(int i=0;i<fields.size();i++) {
      FieldInfo fieldInfo = (FieldInfo)fields.elementAt(i);

      if((getField(i).getAccessFlags() & AccessFlags.STATIC) == staticFlag) {
	if((fieldInfo.getName().equals(name)) &&
	   (fieldInfo.getSignature().equals(type))) 
	  return cnt;

	cnt++;
      }
    }
    return -1;
  }

  // return number of non-static fields of this class
  public int nonStaticFields() {
    int sum = 0;
    
    for(int i=0;i<fields.size();i++) 
      if((getField(i).getAccessFlags() & AccessFlags.STATIC) == 0)
	sum++;

    // add fields of super classes (only for non-native classes)
    if(getSuperClassIndex() < NativeMapper.lowestNativeId) 
      sum += ClassLoader.getClassInfo(getSuperClassIndex()).nonStaticFields();

    return sum;
  }

  public int getSuperClassIndex() {
    int index = ClassLoader.getClassIndex(getSuperClassName());

    // no such class in the local classes, try native
    if(index < 0)
      index = NativeMapper.getNativeClassId(getSuperClassName());

    return index;
  }

  /**
   * Specify the source file for this class, if it is known.
   */
  public void setSourceFile(String sourceFile) {
    this.sourceFile = sourceFile;
  }
  
  /**
   * Get the source file for this class.  If the file is not known, this
   * may return null.
   */
  public String getSourceFile() {
    return sourceFile;
  }
  
  /**
   * Set the inner class information of this class.  Using inner classes is 
   * a bit tricky. The array should include elements for every inner class 
   * of this class, and every class that contains this class. For example:
   *
   *<pre>class Foo {
   *    class Bar {
   *        class Quux {
   *            class Squish {
   *            }
   *        }
   *    }
   *    class Baz {
   *    }
   *}</pre>
   *
   * If this ClassInfo is for Foo.Bar, then the inner classes array should be 
   * {FooInfo, BarInfo, QuuxInfo}. The order is important: outer classes 
   * <strong>must</strong> come before their inner classes. Note that the 
   * current class must have an entry in the array. Squish and Baz may be
   * included in the array, but they're not necessary.
   *
   * @see InnerClassInfo
   */
  public void setInnerClasses(InnerClassInfo[] innerClasses) {
    this.innerClasses = innerClasses;
  }
  
  /**
   * Get the inner class information of this class.
   *
   * @see #setInnerClasses()
   */
  public InnerClassInfo[] getInnerClasses() {
    return innerClasses;
  }
}
