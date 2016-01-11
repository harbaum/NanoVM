package nanovm.ctbot.drivers;

public class DistanceSensor
{
  public static final int MAXIMUM = 255;
  public static final int MINIMUM = 0;
  public static native void updateLeft(); // takes some time
  public static native void updateRight(); // takes some time
  public static native int getLeft();
  public static native int getRight();
  public static native void setEnabled(boolean enable);
  public static native boolean getEnabled();
}


