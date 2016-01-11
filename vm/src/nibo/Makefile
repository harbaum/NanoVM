#
# Makefile include for NanoVM core files
#

NIBO_DIR = $(ROOT_DIR)/vm/src/nibo
NIBO_OBJS = \
  nibo/adc.o \
  nibo/avrisp.o \
  nibo/display.o \
  nibo/gfx.o \
  nibo/delay.o \
  nibo/i2c_master.o \
  nibo/pwm.o \
  nibo/motco.o \
  nibo/distco.o \
  nibo/native_impl.o \
  nibo/native_bot.o \
  nibo/native_clock.o \
  nibo/native_graphicdisplay.o \
  nibo/native_textdisplay.o \
  nibo/native_distancesensor.o \
  nibo/native_edgedetector.o \
  nibo/native_leds.o \
  nibo/native_linedetector.o \
  nibo/native_motor.o \
  nibo/native_wheelencoder.o \
  nibo/native_irtransceiver.o \
  


OBJS += $(NIBO_OBJS)

# convert and upload a class file

nibo/%.o:$(NIBO_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

nibo/%.d:$(NIBO_DIR)/%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@
