#
# Makefile include for NanoVM core files
#

CC65_DIR = $(ROOT_DIR)/vm/src/cc65
CC65_OBJS = \
  cc65/native_impl.o \
  


OBJS += $(CC65_OBJS)

# convert and upload a class file

cc65/%.o:$(CC65_DIR)/%.c Makefile
	$(COMPILE-CMD)

cc65/%.d:$(CC65_DIR)/%.c
	$(DEP-GEN-CMD)
