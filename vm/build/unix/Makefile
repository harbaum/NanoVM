#
#  Makefile for unix version of the NanoVM
#

PROJ = NanoVM
CONFIG = UnixTest.config

DEFAULT_FILE = FormatterTest
#DEFAULT_FILE = FloatTest2
#DEFAULT_FILE = Int31Test
#DEFAULT_FILE = Switch
#DEFAULT_FILE = MathTest
#DEFAULT_FILE = Inheritance

CFLAGS += -DDEBUG

ROOT_DIR = ../../..

# build intel hex files
all: $(PROJ)

include ../../src/Makefile
include ../../src/unix/Makefile

# general GCC compiler flags
CFLAGS += -std=gnu99 -Wall -Werror -pedantic-errors -Wundef -Wno-long-long

# host compiler and linker flags
CFLAGS += -Os -DUNIX -I. -DVERSION="\"$(VERSION)\""
CLDFLAGS += -lm

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

# avr specific entries
$(PROJ): $(OBJS)
	$(CC) $(CLDFLAGS) -o $@ $(OBJS)

%.o: %.c Makefile
	$(COMPILE-CMD)

%.d:%.c
	$(DEP-GEN-CMD)

# just run target from java directory
%-run: $(ROOT_DIR)/java/examples/%.java $(PROJ)
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$*.java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar $(ROOT_DIR)/tool/config/UnixTest.config $(ROOT_DIR)/java/examples $*
	./$(PROJ) $*.nvm

%-debug: $(ROOT_DIR)/java/examples/%.java $(PROJ)
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$*.java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar $(ROOT_DIR)/tool/config/UnixTest.config $(ROOT_DIR)/java/examples $*
	./$(PROJ) -d $*.nvm

# run target from java dir and verify with sun-jvm output
%-verify: $(ROOT_DIR)/java/examples/%.java $(PROJ)
	javac -classpath "$(ROOT_DIR)/java$(PATH_SEP)$(ROOT_DIR)/java/examples" $(ROOT_DIR)/java/examples/$*.java
	java -jar $(ROOT_DIR)/tool/NanoVMTool.jar $(ROOT_DIR)/tool/config/UnixTest.config $(ROOT_DIR)/java/examples $*
	./$(PROJ) -q $*.nvm > $(PROJ).log
	java -cp $(ROOT_DIR)/java/examples $* > java.log
	@if [ "`diff $(PROJ).log java.log`" != "" ]; then \
		echo ">>>>>>>>>>>> Verify failed!!! <<<<<<<<<<<<<<"; \
	else \
		echo "Verify ok."; \
	fi
	@rm $(PROJ).log java.log

clean:
	rm -f *.d *.o *~ nvmdefault.h

include $(OBJS:.o=.d)
