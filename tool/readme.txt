NanoVMTool.jar - Classfile converter and Uploader for NanoVM 
http://www.harbaum.org/till/nanovm
----------------------------------------------------------------------------

The NanoVMTool is a helper tool for the NanoVM. It is required to
convert class files into the internal NanoVM format and to upload them
to the target device. This has several advantages:

- The total file size is reduced
- All depending class files are combined into one big file
- Code optimizations can take place

License
-------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA

Parts of this tool are based on public domain code written by
Kimberley Burchett. See http://www.kimbly.com/code/classfile/ for for
infos. I have heavily hacked around her code, so don't blame her if
you think my code is ugly.

The NanoVM
----------

In order to use the converter the NanoVM has to be installed on the
target system. See http://www.harbaum.org/till/nanovm for a copy of
the NanoVM itself. The NanoVM is correctly installed and working
properly if the built-in demo-application runs correctly. The Asuro
demo application e.g. prints some output via the IR serial interface
and then cycles through a simple LED test application.

Javax.comm/RXTX/gnu.io
----------------------

The NanoVMTool has been written in Java, therefore it runs e.g. under
Linux and Windows. To allow the converter to upload files directly to
the target like e.g. the Asuro, the Java running on the PC needs to
get support to access the PCs serial ports.

In early versiosn of the nanovm this was done by installing the
javax.comm package. The usage of this package is deprecated, since
there isn't any Windows support anymore, there has never been MacOS
support and usable Linux support has only been added recently.

Instead the rxtx/gnu.io is being used (see http://www.rxtx.org/ for
details). The NanoVM provides a script in nanovm/doc/install_rxtx.sh
to support the installation of rxtx under Linux. Further instructions
can be found at http://www.jcontrol.org/download/readme_rxtx_en.html.

Without rxtx/gnu.io NanoVMTool will be unable to upload the file to
the target system.

Running NanoVMTool
------------------

NanoVMTool has been tested under Linux, Windows and MacOS. The basic
syntax to call NanoVMTool is:

java -jar NanoVMTool.jar CONFIG CLASSPATH CLASS

CONFIG is a file containing a target description. It includes
information about the supported native classes and the hardware of the
target system incl. the interface used for upload. There are two files
for the Asuro: Asuro.config and AsuroWin.config, both differ in the
name of the serial device used on the PC (COM1 vs. /dev/ttyS0).

CLASSPATH is the path to the NanoVM classes to be installed.

CLASS is the name of the class to be installed (e.g. AsuroLED).

Running NanoVMTool under Linux
------------------------------

The LED demo is being converted and uploaded under Linux using
e.g. the command:

java -jar NanoVMTool.jar Asuro.config ../examples AsuroLED

Running NanoVMTool under Windows
--------------------------------

The archive contains a simple batch file (asuro_upload.bat). You'll
manually have to adjust the both paths in this file and can then
easily convert files just by typing:

convert AsuroLED

Example files
-------------

Example java files for use with the Asuro can be found at
http://www.harbaum.org/till/nanovm
