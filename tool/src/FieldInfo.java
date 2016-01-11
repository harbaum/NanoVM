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
* A class for storing information about fields.
*
* @see ClassInfo#addField()
*/
public class FieldInfo extends CommonInfo {
	private String signature;
	private Object constValue;
	private boolean synthetic;

	public FieldInfo(short accessFlags, String name, String signature) {
		setAccessFlags(accessFlags);
		setName(name);
		setSignature(signature);
	}

	/**
	* Set the signature of the field.  For example "[Ljava/lang/String;"
	* 
	* @see Signature
	*/
	public void setSignature(String signature) {
		this.signature = signature;
	}

	/**
	* Get the signature of the field.
	* 
	* @see Signature
	*/
	public String getSignature() {
		return signature;
	}

	/**
	* Set the constant value of this field.  This only applies to final fields.
	* The value must be a String, Integer, Long, Float, or Double.
	*/
	public void setConstantValue(Object constValue) {
		this.constValue = constValue;
	}

	/**
	* Get the constant value of this field.  The result must be a String,
	* Integer, Long, Float, or Double.
	*/
	public Object getConstantValue() {
		return constValue;
	}

	public void setSynthetic(boolean synthetic) {
		this.synthetic = synthetic;
	}

	public boolean isSynthetic() {
		return synthetic;
	}
}
