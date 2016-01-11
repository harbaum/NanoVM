#
# Makefile include for NanoVM core files
#

UNIX_DIR = $(ROOT_DIR)/vm/src/unix
UNIX_OBJS = \
  unix/native_impl.o \
  


OBJS += $(UNIX_OBJS)

# convert and upload a class file

unix/%.o:$(UNIX_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

unix/%.d:$(UNIX_DIR)/%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@
