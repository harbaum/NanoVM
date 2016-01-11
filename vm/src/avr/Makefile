#
# Makefile include for NanoVM core files
#

AVR_DIR = $(ROOT_DIR)/vm/src/avr
AVR_OBJS = \
  avr/native_avr.o \
  avr/native_impl.o \
  


OBJS += $(AVR_OBJS)

# convert and upload a class file

avr/%.o:$(AVR_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

avr/%.d:$(AVR_DIR)/%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@
