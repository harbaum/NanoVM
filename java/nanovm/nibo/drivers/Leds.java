package nanovm.nibo.drivers;

/**
 * Provides methods to control the LEDs.
 * @author Nils Springob
 */
public class Leds {
  public static final int LEFT        = 0;
  public static final int LEFT_FRONT  = 1;
  public static final int FRONT_LEFT  = 2;
  public static final int FRONT_RIGHT = 3;
  public static final int RIGHT_FRONT = 4;
  public static final int RIGHT       = 5;

  public static final int OFF    = 0x00;
  public static final int GREEN  = 0x01;
  public static final int RED    = 0x02;
  public static final int ORANGE = 0x03;

  public static final int MINIMUM_BRIGHTNESS = 0;
  public static final int MAXIMUM_BRIGHTNESS = 1023;

  /**
   * set color for a given status LED
   * @param led id of the status LED
   * @param color color for the status LED
   */
  public static native void setStatus(int led, int color);
  
  /**
   * get color of a given status LED
   * @param led id of the status LED
   * @return color of the status LED
   */
  public static native int getStatus(int led);
  
  /**
   * set brightness for the headlight LEDs
   * @param brightness 0 for dark 1023 for maximum brightness
   */
  public static native void setHeadlights(int brightness);
  
  /**
   * get brightness of the headlight LEDs
   * @return 0 for dark 1023 for maximum brightness
   */
  public static native int getHeadlights();
  
  /**
   * set brightness for the display LEDs
   * @param brightness 0 for dark 1023 for maximum brightness
   */
  public static native void setDisplaylight(int brightness);
  
  /**
   * get brightness of the display LEDs
   * @return 0 for dark 1023 for maximum brightness
   */
  public static native int getDisplaylight();
  
}


