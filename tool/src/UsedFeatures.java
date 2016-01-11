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
* A class for storing information about methods.
*/

public class UsedFeatures {
  static final int LOOKUPSWITCH = (1<<0);
  static final int TABLESWITCH  = (1<<1);
  static final int BIT32        = (1<<2);
  static final int FLOAT        = (1<<3);
  static final int ARRAY        = (1<<4);
  static final int INHERITANCE  = (1<<5);
  static final int EXTSTACK     = (1<<6);

  private static int features;

  public static void add(int f){
    features |= f;
  }
  
  public static int get(){
    System.out.println("Feature value is 0x"+Integer.toHexString(features));
    return features;
  }

}
