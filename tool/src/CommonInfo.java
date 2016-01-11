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
* A common base class for ClassInfo, MethodInfo, and FieldInfo.
*
* @see ClassInfo
* @see MethodInfo
* @see FieldInfo
*/
public class CommonInfo {
  short accessFlags = 0;
  Vector attributes = new Vector();
  String name;
  
  /**
   * Set the name of this class/method/field.
   */
  public void setName(String name) {
    this.name = name;
  }
  
  /**
   * Get the name of this class/method/field.
   */
  public String getName() {
    return name;
  }
  
  /**
   * Set the access flags of the class.
   * 
   * @see AccessFlags
   */
  public void setAccessFlags(short accessFlags) {
    this.accessFlags = accessFlags;
  }
  
  /**
   * Get the access flags of the class.
   * 
   * @see AccessFlags
   */
  public short getAccessFlags() {
    return accessFlags;
  }
  
  /**
   * Add a <i>non-standard</i> attribute.  This does not include the Code 
   * attribute for methods, the SourceFile attribute for classes, or the 
   * ConstantValue attribute for fields.
   */
  public void addAttribute(AttributeInfo attributeInfo) {
    attributes.addElement(attributeInfo);
  }
  
  /**
   * Get all <i>non-standard</i> attributes.  This does not include the Code 
   * attribute for methods, the SourceFile attribute for classes, or the 
   * ConstantValue attribute for fields.
   */
  public AttributeInfo[] getAttributes() {
    AttributeInfo[] list = new AttributeInfo[attributes.size()];
    attributes.copyInto(list);
    return list;
  }
}
