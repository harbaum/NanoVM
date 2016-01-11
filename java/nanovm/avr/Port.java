//
// nanovm/avr/Port.java
//
// the contents of this file is actually never realy used
// since class2uvm replaces these calls with native calls
//

package nanovm.avr;

public class Port {
//  public static final boolean INPUT  = false;
//  public static final boolean OUTPUT = true;

  public native void setInput(int bit);
  public native void setOutput(int bit);
  public native void setBit(int bit);
  public native void clrBit(int bit);
}
