package nanovm.ctbot.drivers;

public class Display
{
  public static final int MIN_X = 0;
  public static final int MAX_X = 19;
  public static final int MIN_Y = 0;
  public static final int MAX_Y = 3;
  public static final int HIDDEN = 0;
  public static final int BLOCK = 1;
  public static final int BLINKBLOCK = 2;
  public static final int LINE = 3;
  public static native void clear();
  public static native void gotoXY(int x, int y);
  public static native void print(String s);
  public static native void setCursorMode(int mode);
}

