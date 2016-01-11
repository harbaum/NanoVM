//
// nanovm/avr/AVR.java
//
// part of NanoVM
//

package nanovm.avr;

public class AVR {
  // these are fields representing ports the vm 
  // handles internally
  public static Port portA;
  public static Port portB;
  public static Port portC;
  public static Port portD;
  public static Port portE;
  public static Port portF;
  public static Port portG;
  public static Port portH;

  // and the PWM units
  public static Pwm pwm0;
  public static Pwm pwm1;

  // return system clock in khz
  static public native int getClock();
}
