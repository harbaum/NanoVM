package nanovm.ctbot.drivers;

public class LightBarrier
{
  public static native boolean getState();
  public static native void setEnabled(boolean enable);
  public static native boolean getEnabled();
}


