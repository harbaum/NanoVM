package nanovm.ctbot.drivers;

public class Motor
{
  public static final int MAXIMUM = 500;
  public static final int MINIMUM = -500;
  public static final int MAXIMUM_SPEED = 110;
  public static final int MINIMUM_SPEED = -110;
  public static native void setLeft(int val);
  public static native void setRight(int val);
  public static native int getLeft();
  public static native int getRight();
  public static native void setLeftSpeed(int val);
  public static native void setRightSpeed(int val);
  public static native int getLeftSpeed();
  public static native int getRightSpeed();
  public static native void stop();
}

