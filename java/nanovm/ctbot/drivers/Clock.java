package nanovm.ctbot.drivers;

public class Clock
{
  public static native int getSeconds();
  public static native int getMilliSeconds();
  public static native void delayMilliseconds(int ms);
  public static native void delayMicroseconds(int us);
}


