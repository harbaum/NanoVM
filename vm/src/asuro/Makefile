#
# Makefile include for NanoVM core files
#

ASURO_DIR = $(ROOT_DIR)/vm/src/asuro
ASURO_OBJS = \
  asuro/native_asuro.o \
  asuro/native_impl.o \
  


OBJS += $(ASURO_OBJS)

# convert and upload a class file

asuro/%.o:$(ASURO_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

asuro/%.d:$(ASURO_DIR)/%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@
