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
* A class for storing information about the exception table of a method.
* This class represents one row in the ExceptionTable.
*
* @see sli.kim.classfile.CodeInfo#setExceptionTable()
*/
public class ExceptionInfo {
	public short startPC, endPC, handlerPC;
	public String catchType;

	/**
	* Construct an ExceptionInfo
	*
	* @param startPC the index of the first instruction in the try block (that is,
	*    startPC is inclusive).
	* @param endPC the index of the first instruction not in the try block (that is,
	*    endPC is exclusive).  If the try block extends to the end of the method, 
	*    endPC will be equal to the length of the bytecode for the method.
	* @param handlerPC the index of the first instruction of the catch handler.
	* @param catchType the name of the exception class caught by the catch 
	*    handler.  If this parameter is null, the handler catches all exceptions (this
	*    is used for finally blocks).
	*/
	public ExceptionInfo(short startPC, short endPC, short handlerPC, String catchType) 
	{
		this.startPC = startPC;
		this.endPC = endPC;
		this.handlerPC = handlerPC;
		this.catchType = catchType;
	}
}
