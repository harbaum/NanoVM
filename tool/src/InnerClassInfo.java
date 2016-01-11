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

/**
* A class for storing information about inner classes.
* This class represents one row in the InnerClasses table.
*
* @see sli.kim.classfile.ClassInfo#setInnerClasses()
*/
public class InnerClassInfo {
	public String innerClass, outerClass, simpleName;
	public short flags;

	/**
	* Construct an inner class record.
	*
	* @param innerClass the full name of the inner class (eg "mypackage.Foo$Bar")
	* @param outerClass the full name of the outer class.  This is null for non-member
	*    classes (that is, classes that are declared static)
	* @param simpleName the simple name of the class (eg "String").  This is null for
	*    anonymous classes.
	* @param the access flags of the class.  When an inner class is compiled to a .class
	*    file, the compiler changes its access flags to ensure compatibility with pre-1.1
	*    virtual machines.  Specifically, protected becomes public, and private becomes 
	*    package.
	*/
	public InnerClassInfo(String innerClass, String outerClass, String simpleName, short flags) {
		this.innerClass = innerClass;
		this.outerClass = outerClass;
		this.simpleName = simpleName;
		this.flags = flags;
	}
}
