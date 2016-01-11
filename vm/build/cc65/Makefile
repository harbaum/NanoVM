#
#  Makefile for cc65 version of the NanoVM
#

DEBUG  = 0
TARGET = apple2enh

DEFAULT_FILE = HelloWorld
#DEFAULT_FILE = icmp
#DEFAULT_FILE = count
#DEFAULT_FILE = arithmetic
#DEFAULT_FILE = Switch
#DEFAULT_FILE = MethodCall
#DEFAULT_FILE = StaticTest
#DEFAULT_FILE = StringAndHeapTest
#DEFAULT_FILE = SelfInstance
#DEFAULT_FILE = OneClass
#DEFAULT_FILE = Inheritance
#DEFAULT_FILE = DivByZero
#DEFAULT_FILE = QuickSort
#DEFAULT_FILE = Erathostenes
#DEFAULT_FILE = ConsoleEcho
#DEFAULT_FILE = Rot13

ROOT_DIR = ../../..
CONFIG   = cc65.config

# cc65 compiler setup
AS = ca65
CC = cc65
LD = ld65

# cc65 assembler, compiler and linker flags
AFLAGS  = -t $(TARGET)
CFLAGS  = -t $(TARGET) -O --register-vars --static-locals --codesize 400 -I. -DVERSION="\"$(VERSION)\""
LDFLAGS = -t $(TARGET) -m NanoVM.map

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG
endif

all: NanoVM

include ../../src/Makefile
include ../../src/cc65/Makefile

nvmdefault.h: Makefile

# define canned command sequences
define COMPILE-CMD
$(CC) $(CFLAGS) --create-dep $< -o $(@:.o=.s)
@$(AS) $(AFLAGS) $(@:.o=.s) -o $@
@sed 's!.s:!.o:!' < $(@:.o=.u) > $(@:.o=.d)
@rm -f $(@:.o=.s) $(@:.o=.u)
endef

define DEP-GEN-CMD
endef

# cc65 specific entries
NanoVM: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(TARGET).o $(OBJS) $(TARGET).lib

%.o: %.c Makefile
	$(COMPILE-CMD)

%.d:%.c
	$(DEP-GEN-CMD)

%.nvm: $(ROOT_DIR)/java/examples/%.java
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$*.java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar $(ROOT_DIR)/tool/config/$(CONFIG) $(ROOT_DIR)/java/examples $*

clean:
	rm -f *.d *.o *.map *~ nvmdefault.h

include $(OBJS:.o=.d)
