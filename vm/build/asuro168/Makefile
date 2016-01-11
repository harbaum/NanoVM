#
#  Makefile for the Asuro version of the NanoVM
#

CONFIG = Asuro.config
DEVICE = atmega168
DEFAULT_FILE = AsuroLED
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
CFLAGS += -Os -ffunction-sections -DAVR -I. -mmcu=$(DEVICE) -DVERSION="\"$(VERSION)\""
CLDFLAGS += -Wl,--gc-sections -T$(ROOT_DIR)/vm/build/ldscripts/avr4_m168_keep.x -mmcu=$(DEVICE)

# build intel hex files
all: NanoVM.hex NanoVM_eeprom.hex

include ../../src/Makefile
include ../../src/asuro/Makefile

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
	$(CC) $(CLDFLAGS) -o $@ $(OBJS)
	avr-size -A $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

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
	uisp  -dprog=stk200 --erase  --wr_fuse_l=0xbf --wr_fuse_h=0xd9
	uisp --segment=flash  -dprog=stk200 --upload if=NanoVM.hex --verify
	uisp --segment=eeprom  -dprog=stk200 --upload if=NanoVM_eeprom.hex --verify

avrdude: NanoVM.hex NanoVM_eeprom.hex
	avrdude -c stk500v2 -p atmega168 -B 10 -U lfuse:w:0xbf:m -U hfuse:w:0xd9:m -U flash:w:NanoVM.hex -U eeprom:w:NanoVM_eeprom.hex

clean::
	rm -f *.d *.o asuro/*.d asuro/*.o avr/*.d avr/*.o *~ nvmdefault.h *.elf

include $(OBJS:.o=.d)
