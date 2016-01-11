#
#  Makefile for the Asuro version of the NanoVM
#

CONFIG = Ctbot.config
DEVICE = atmega32

#DEFAULT_FILE = ctbot/TimeTest
#DEFAULT_FILE = ctbot/IrDrive
#DEFAULT_FILE = ctbot/IrTest
#DEFAULT_FILE = ctbot/MouseTest
DEFAULT_FILE = ctbot/FollowLineDemo
#DEFAULT_FILE = ctbot/MotorSpeedTest
#DEFAULT_FILE = ctbot/NavigatorTest
#DEFAULT_FILE = ctbot/DistTest

ROOT_DIR = ../../..
# CFLAGS += -DDEBUG

# general GCC compiler flags
CFLAGS += -std=gnu99 -Wall -Werror -pedantic-errors -Wundef -Wno-long-long

# AVR compiler setup
PREFIX =
CC = $(PREFIX)avr-gcc
OBJCOPY = $(PREFIX)avr-objcopy
OBJDUMP = $(PREFIX)avr-objdump

# AVR compiler and linker flags
CFLAGS += -Os -ffunction-sections -I. -mmcu=$(DEVICE) -DVERSION="\"$(VERSION)\"" -I$(ROOT_DIR)/vm/src
CLDFLAGS += -v -mmcu=$(DEVICE)

# MATH lib
MATH = -lm
#MATH =

# build intel hex files
all: NanoVM.hex

#OBJS += ctbot/shift.o

include ../../src/Makefile

include ../../src/ctbot/Makefile

#OBJS = $(NVM_OBJS)
# ctbot/shift.o

nvmdefault.h: Makefile

# define canned command sequences
define COMPILE-CMD
$(CC) $(CFLAGS) -c $< -o $@
endef

define DEP-GEN-CMD
set -e; $(CC) -MM $(CFLAGS) $< \
| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@ ; \
[ -s $@ ] || rm -f $@
endef

%.o: %.c Makefile
	$(COMPILE-CMD)

%.d:%.c
	$(DEP-GEN-CMD)

# avr specific entries
%.elf: $(OBJS)
	$(CC) $(CLDFLAGS) -o $@ $(OBJS) $(MATH)
	avr-size -A NanoVM.elf


%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


NanoVM.lss: NanoVM.elf
	avr-objdump -h -S NanoVM.elf >NanoVM.lss

lss: NanoVM.lss



# Fuse high byte:
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (full output swing)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ WDTON (WDT not always on)
#        +-------------------- RSTDISBL (reset pin is enabled)
# Fuse low byte:
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ BODEN (BrownOut Detector enabled)
#        +-------------------- BODLEVEL (2.7V)

# use uisp tool to configure whole cpu
uisp: NanoVM.hex NanoVM_eeprom.hex
	uisp  -dprog=stk500 --erase  --wr_fuse_l=0xbf --wr_fuse_h=0xd9
	uisp --segment=flash  -dprog=stk500 --upload if=NanoVM.hex --verify
	uisp --segment=eeprom  -dprog=stk500 --upload if=NanoVM_eeprom.hex --verify

avrdude: NanoVM.hex 
	avrdude -c stk500v2 -p m32 -B 1 -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U flash:w:NanoVM.hex

clean:
	rm -f *.d *.o *~ nvmdefault.h *.elf ctbot/*.d ctbot/*.o $(ROOT_DIR)/java/examples/nanovm/ctbot/utils/*.class

include $(OBJS:.o=.d)

.PHONY: clean
.SECONDARY: NanoVM.hex $(OBJS)
