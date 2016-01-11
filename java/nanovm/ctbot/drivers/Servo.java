package nanovm.ctbot.drivers;

public class Servo
{
  public static final int MAXIMUM = 25;
  public static final int MINIMUM = 0;
  public static native void set1(int val);
  public static native void set2(int val);
  public static native int get1();
  public static native int get2();
}

