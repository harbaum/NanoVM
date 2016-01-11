#
# Makefile include for NanoVM core files
#

CTBOT_DIR = $(ROOT_DIR)/vm/src/ctbot
CTBOT_OBJS = \
  ctbot/shift.o \
  ctbot/display.o \
  ctbot/delay.o \
  ctbot/pwm.o \
  ctbot/pid.o \
  ctbot/adc.o \
  ctbot/mouse.o \
  ctbot/wheelencoder.o \
  ctbot/ir_rc5.o \
  ctbot/native_impl.o \
  ctbot/native_bot.o \
  ctbot/native_clock.o \
  ctbot/native_display.o \
  ctbot/native_distancesensor.o \
  ctbot/native_edgedetector.o \
  ctbot/native_ldrsensor.o \
  ctbot/native_leds.o \
  ctbot/native_lightbarrier.o \
  ctbot/native_linedetector.o \
  ctbot/native_motor.o \
  ctbot/native_mouse.o \
  ctbot/native_servo.o \
  ctbot/native_shuttersensor.o \
  ctbot/native_wheelencoder.o \
  ctbot/native_irreceiver.o \
  

# ctbot/ct-Bot.o \



OBJS += $(CTBOT_OBJS)

# convert and upload a class file

ctbot/%.o:$(CTBOT_DIR)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

ctbot/%.d:$(CTBOT_DIR)/%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@
