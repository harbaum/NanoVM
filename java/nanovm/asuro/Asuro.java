//
// nanovm/asuro/Asuro.java
//
// When converting NanoVM code using the Convert tool, this
// code will magically be replaced by native methods. This
// code will never be called.
//
// It is possible to fill this code with e.g. a graphical 
// representation of the Asuro allowing the Java code to be
// run on a PC using a Asuro simulation
//

package nanovm.asuro;

public class Asuro {
  public final static int OFF       = 0;
  public final static int ON        = 1;

  public final static int RED       = 1;
  public final static int GREEN     = 2;
  public final static int YELLOW    = 3;

  public final static int LEFT      = 0;
  public final static int RIGHT     = 1;

  public final static int ANY       = 0;
  public final static int SELECTIVE = 1;

  public static native void statusLED(int state);
  public static native void wait(int msec);
  public static native void motor(int left, int right);
  public static native void lineLED(int state);
  public static native void backLED(int left, int right);
  public static native int lineSensor(int sensor);
  public static native int motorSensor(int sensor);

  // mode can be "ANY" which returns 1 if any of the switches
  // is pressed and 0 if none of them is. or mode can be
  // SELECTIVE which returns an _inprecise_ bitmap of the
  // single buttons
  public static native int getSwitches(int mode);
}
