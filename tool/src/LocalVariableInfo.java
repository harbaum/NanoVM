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
* A class for storing information about local variables.
* This class represents one row in the LocalVariableTable.
*
* @see sli.kim.classfile.CodeInfo#setLocalVariableTable()
*/
public class LocalVariableInfo {
	public short startPC, length, slot;
	public String name, signature; 

	public LocalVariableInfo(short startPC, short length, String name,
		String signature, short slot) 
	{
		this.startPC = startPC;
		this.length = length;
		this.name = name;
		this.signature = signature;
		this.slot = slot;
	}
}
