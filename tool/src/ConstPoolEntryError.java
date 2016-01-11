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
* An instance of this class is thrown when a ConstPoolEntry is 
* used incorrectly.  For example, calling setUTF(null), or calling 
* getClassName() on an entry of type INT.  It is a subclass of 
* Error instead of Exception because checking for it all the time 
* would make working with ConstPoolEntries very tedious.
*/
public class ConstPoolEntryError extends Error {
	public ConstPoolEntryError(String msg) {
		super(msg);
	}
}
