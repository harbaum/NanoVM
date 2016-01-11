package nanovm.ctbot.drivers;

public class Mouse
{
  public static final int MAXIMUM = Integer.MAX_VALUE;
  public static final int MINIMUM = Integer.MIN_VALUE;
  public static final int QUAL_MAX = 255;
  public static final int QUAL_MIN = 0;
  public static final int PIXEL_X = 19;
  public static final int PIXEL_Y = 19;
  public static native int getX();
  public static native int getY();
  public static native int getQuality();

  public static native void requestPicture();
  public static native int getPixel();

  public static native void reset();
}

