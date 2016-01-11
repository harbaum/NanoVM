package nanovm.ctbot.drivers;

public class Leds
{
  public static final int BLUE_RIGHT  = 0x01;
  public static final int BLUE_LEFT   = 0x02;
  public static final int RED    = 0x04;
  public static final int ORANGE = 0x08;
  public static final int YELLOW = 0x10;
  public static final int GREEN  = 0x20;
  public static final int BLUE   = 0x40;
  public static final int WHITE  = 0x80;
  public static native void set(int val);
  public static native int get();
}


