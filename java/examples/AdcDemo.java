/*
    ADCDemo.java

    Simple analog digital conversion (ADC) demo. Run this e.g. on the
    simple avr atmega8 demo board
 */

import nanovm.avr.*;

class AdcDemo {
  public static void main(String[] args) {
    System.out.println("Analog digital conversion demo");

    // no need to initialize the timer, since 100Hz is default.

    // initialize the ADC conversion (there's no default setup for ADC)
    Adc.setPrescaler(Adc.DIV64);     // slow but accurate conversion
    Adc.setReference(Adc.INTERNAL);  // use internal reference

    // permanently poll ADC channel0
    while(true) {
      // display internal VBG voltage (about 1.23 volts)
      System.out.println("Value VBG: " + Adc.getValue(Adc.CHANNELVBG));

      // display internal GND (0 volts)
      System.out.println("Value Gnd: " + Adc.getValue(Adc.CHANNELGND));

      // display ADC0 value with 10 bits resolution
      System.out.println("Value CH0: " + Adc.getValue(Adc.CHANNEL0));

      // display ADC0 value with 8 bits resolution
      System.out.println("Byte CH0:  " + Adc.getByte(Adc.CHANNEL0));

      // wait a second for next run
      Timer.wait(100);
    }
  }
}

     
