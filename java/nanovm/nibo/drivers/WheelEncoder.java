package nanovm.nibo.drivers;

/**
 * Encapsulates the information about the wheels rotation. You will have to call
 * the update Method first to get actual values. All wheel encoder information
 * is given in ticks, a tick is 1/20 of a whole rotation.
 * @author Nils Springob
 */
public class WheelEncoder {

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
  public static final int MAXIMUM_SPEED = 200;

  /**
   * maximum value for speed
   */
  public static final int MINIMUM_SPEED = -200;

  
  /**
   * update wheelencoder data
   */
  public static native void update();
  
  /**
   * get tick-count from left wheel since last call
   * @return ticks since last call
   */
  public static native int getLeftInc();

  /**
   * get tick-count from right wheel since last call
   * @return ticks since last call
   */
  public static native int getRightInc();

  /**
   * get actual speed (ticks/second) from left wheel
   * @return ticks per second
   */
  public static native int getLeftSpeed();

  /**
   * get actual speed (ticks/second) from right wheel
   * @return ticks per second
   */
  public static native int getRightSpeed();

  /**
   * enable and disable the wheel-encoder
   * @param enable true for enable
   */
  public static native void setEnabled(boolean enable);

  /**
   * get status of wheel-encoder
   * @return true when enabled
   */
  public static native boolean getEnabled();

}


