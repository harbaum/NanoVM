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

//
// MethodIdTable.java
//

public class MethodIdTable {
  private static int[] mindex;

  // build the complete method id table
  public static void build() {
    mindex = new int[ClassLoader.totalMethods()];

    // clear index table
    for(int i=0;i<ClassLoader.totalMethods();i++) mindex[i] = -1;

    // generate table
    for(int i=0,id = 0;i<ClassLoader.totalMethods();i++) {
      // entry has not been set yet
      if(mindex[i] == -1) {
	// use same id on all methods mit same name and signature
	for(int j = i;j<ClassLoader.totalMethods();j++) {
	  if(ClassLoader.getMethod(i).getName().equals(
	       ClassLoader.getMethod(j).getName()) &&
	     ClassLoader.getMethod(i).getSignature().equals(
	       ClassLoader.getMethod(j).getSignature())) {
	    
	    mindex[j] = id;
	  }
	}
	id++;
      }
    }
  }

  // get one entry from the method id table
  public static int getEntry(int i) {
    return mindex[i];
  }

}
