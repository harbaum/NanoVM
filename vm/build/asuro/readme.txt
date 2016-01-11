NanoVM for Asuro - (c) 2005 by Till Harbaum <till@harbaum.org>
--------------------------------------------------------------

The NanoVM for Asuro is an implementation of the java vm for the Asuro
robot and other AVR based devices. It allows you to run simple java 
programs on the Asuro.

See http://www.harbaum.org/till/NanoVM for details.

License
-------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Installation
------------

Unlike other software for the Asuro, the NanoVM is a complete replacement
for the Asuros firmware. This means, that it cannot be uploaded using the 
Asuros built-in bootloader. Instead, the CPU has to be completely 
reprogrammed using a seperate programming hardware like the STK-500 board
sold by Atmel.

Buy a seperate CPU!!!!
----------------------

Furthermore it is strongly recommended to use a new Mega8 CPU (these go for
around $3, so there's really no reason not to buy a seperate CPU for the
NanoVM) instead of the one provided with your Asuro. This way, you can always
re-install the original CPU and still run the original C demo files that came
with the Asuro. The CPU delivered with the Asuro contains a special boot
loader. This bootloader is not available seperately, therefore replacing it
with the NanoVM will leave you no way to return to the original setup. 

NanoVM upload
-------------

The NanoVM for Asuro (as well as other versions for AVR based devices) comes
in two files:

NanoVM.hex          - the nano vm itself in intel hex format
NanoVM_eeprom.hex   - the eeprom configuration in intel hex format

Use your favorite AVR programming software (e.g. ponyprog unter Windows, 
or uisp under Linux) to install both files on the AVR. The eeprom image
already contains a little demo application that will make the Asuro run 
a LED test and write a few bytes on its serial IR interface. Like the original
firmware, the NanoVM for Asuro uses a bitrate of 2400 bit/s.

Upload of java programs
-----------------------

The upload of new java programs doesn not require any special hardware. The
NanoVM for Asuro contains a bootloader very similar to the one that came
pre-installed on the original Asuro CPU. However, these bootloaders are not
identical and a special software is required on the PC to convert standard
java class files into NanoVM format and to upload them to the Asuro. See
http://www.harbaum.org/till/NanoVM for details.

Order a pre-flashed CPU
-----------------------

If you don't have the programming hardware necessary to install the NanoVM
into a Mega8 CPU you can buy pre-programmed CPUs from me for Euro 10,-. 
Contact me at the address below.

---------------------------------------------------------------------------
Dr. Till Harbaum  <till@harbaum.org>
