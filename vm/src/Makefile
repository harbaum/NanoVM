#
# Makefile include for NanoVM core files
#

VERSION = 1.5

NVM_DIR = $(ROOT_DIR)/vm/src
CFLAGS += -I$(NVM_DIR)
NVM_OBJS = NanoVM.o nvmfile.o vm.o heap.o array.o \
	error.o loader.o native_stdio.o stack.o \
	uart.o debug.o native_lcd.o nvmcomm1.o nvmcomm2.o \
	native_math.o native_formatter.o nvmstring.o \

OBJS += $(NVM_OBJS)

ifeq ($(OS),Windows_NT)
PATH_SEP = ;
else
PATH_SEP = :
endif

./nvmfile.o: ./nvmdefault.h Makefile
./nvmfile.d: ./nvmdefault.h Makefile

nvmdefault.h: $(ROOT_DIR)/java/examples/$(DEFAULT_FILE).java
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$(DEFAULT_FILE).java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar -c -f $@ $(ROOT_DIR)/tool/config/$(CONFIG) $(ROOT_DIR)/java/examples $(DEFAULT_FILE)

# convert and upload a class file
upload-%: $(ROOT_DIR)/java/examples/%.java
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$*.java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar $(ROOT_DIR)/tool/config/$(CONFIG) $(ROOT_DIR)/java/examples $*

%.o:$(NVM_DIR)/%.c Makefile
	$(COMPILE-CMD)

%.d:$(NVM_DIR)/%.c
	$(DEP-GEN-CMD)
