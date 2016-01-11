package nanovm.ctbot.drivers;

public class WheelEncoder
{
  /**
   * maximum value for tick increment
   */
  public static final int MAXIMUM = 32767;

  /**
   * minimum value for tick increment
   */
  public static final int MINIMUM = -32768;

  /**
   * minimum value for speed
   */
  public static final int MAXIMUM_SPEED = 130;

  /**
   * maximum value for speed
   */
  public static final int MINIMUM_SPEED = -130;

  /**
   * get tick-count from left wheel since last call
   */
  public static native int getLeftInc();

  /**
   * get tick-count from right wheel since last call
   */
  public static native int getRightInc();

  /**
   * get actual speed (ticks/second) from left wheel
   */
  public static native int getLeftSpeed();

  /**
   * get actual speed (ticks/second) from right wheel
   */
  public static native int getRightSpeed();

  /**
   * enable and disable the wheel-encoder
   */
  public static native void setEnabled(boolean enable);

  /**
   * get status of wheel-encoder
   */
  public static native boolean getEnabled();

}


