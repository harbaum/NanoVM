//
// nanovm/avr/Adc.java
//

package nanovm.avr;

public class Adc {
  
  // prescaler constants
  public static final int DIV2   = 0;
  public static final int DIV2A  = 1;  // no real use for this
  public static final int DIV4   = 2;
  public static final int DIV8   = 3;
  public static final int DIV16  = 4;
  public static final int DIV32  = 5;
  public static final int DIV64  = 6;
  public static final int DIV128 = 7;

  // set ADC prescaler
  static public native void setPrescaler(int value);

  // voltage reference constants
  public static final int AREF     = 0 << 6;
  public static final int AVCC     = 1 << 6;
//  public static final int RESERVED = 2 << 6;  // don't use
  public static final int INTERNAL = 3 << 6;  

  // set voltage reference
  static public native void setReference(int value);

  // channel constants
  public static final int CHANNEL0   = 0;
  public static final int CHANNEL1   = 1;
  public static final int CHANNEL2   = 2;
  public static final int CHANNEL3   = 3;
  public static final int CHANNEL4   = 4;
  public static final int CHANNEL5   = 5;
  public static final int CHANNEL6   = 6;
  public static final int CHANNEL7   = 7;
  public static final int CHANNELVBG = 14;  // VBG (1.23V)
  public static final int CHANNELGND = 15;  // GND (0V)

  // get current ADC value
  static public native int getValue(int channel);  // get 10 bit value
  static public native int getByte(int channel);   // get 8 bit value
}
