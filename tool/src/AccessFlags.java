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
* Bitfield constants for class/field/method access flags.
*/
public interface AccessFlags {
	public static final short PUBLIC = 0x0001;
	public static final short PRIVATE = 0x0002;
	public static final short PROTECTED = 0x0004;
	public static final short STATIC = 0x0008;
	public static final short FINAL = 0x0010;
	public static final short SYNCHRONIZED = 0x0020;
	public static final short VOLATILE = 0x0040;
	public static final short TRANSIENT = 0x0080;
	public static final short NATIVE = 0x0100;
	public static final short INTERFACE = 0x0200;
	public static final short ABSTRACT = 0x0400;
}
