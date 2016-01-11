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

public class NanoVMTool {
  public static void usage() {
    System.out.println("Usage: NanoVMTool [options] config classpath class");
    System.out.println("Options:");
    System.out.println("    -c        write c header file");
    System.out.println("    -f name   force output file name");
  }
  
  public static void main(String[] args) {
    int curArg = 0;
    boolean writeHeader = false;
    String outputFileName = null;

    System.out.println("NanoVMTool " + Version.version + 
		       " - (c) 2005-2007 by Till Harbaum");

    // parse options
    while((args.length > curArg) && (args[curArg].charAt(0) == '-')) {
      switch(args[curArg].charAt(1)) {
	case 'c':
	  writeHeader = true;
	  break;

	case 'f':
	  outputFileName = args[++curArg];
	  break;

	default:
	  System.out.println("Unknown option " + args[curArg]);
	  usage();
	  return;
      }

      // interpret next option
      curArg++;
    }

    if((args.length - curArg) != 3) {
      usage();
      return;
    }
  
    // load config
    Config.load(args[curArg]);

    // overwrite filename if given on command line
    if(outputFileName != null)
      Config.overwriteFileName(outputFileName);

    ClassLoader.setClassPath(args[curArg+1]);
    ClassLoader.load(args[curArg+2]);    

    System.out.println("Successfully loaded " + 
		       ClassLoader.totalClasses() + " classes");

    // for first tries: write converted file to disk
    UVMWriter writer = new UVMWriter(writeHeader);
  }
}
