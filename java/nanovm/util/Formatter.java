//
// nanovm/util/Formatter.java
//
// When converting NanoVM code using the Convert tool, this
// code will magically be replaced by native methods. This
// code will never be called.
//
// It is possible to fill this code with e.g. a graphical 
// representation of the Asuro allowing the Java code to be
// run on a PC using a Asuro simulation
//

package nanovm.util;

public class Formatter {
  public static native String format(int val, String format);
  public static native String format(boolean val, String format);
  public static native String format(float val, String format);
}
