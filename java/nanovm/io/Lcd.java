//
// nanovm/io/Lcd.java
//
// When converting NanoVM code using the Convert tool, this
// code will magically be replaced by native methods. This
// code will never be called.
//
// It is possible to fill this code with e.g. a graphical 
// representation of the Asuro allowing the Java code to be
// run on a PC using a Asuro simulation
//

package nanovm.io;

public class Lcd {
  public static native void clear();
  public static native void gotoYX(int y, int x);
  public static native void print(String str);
  public static native void print(int num);
  public static native void print(char chr);
}
