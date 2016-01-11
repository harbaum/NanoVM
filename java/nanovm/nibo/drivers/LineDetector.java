package nanovm.nibo.drivers;

/**
 * Encapsulates the information about the two
 * IR-reflex-sensors for line detection. You will have to call the update
 * Method first to get actual values.
 * @author Nils Springob
 */
public class LineDetector {
  /**
   * Maximum value from IR-sensor
   */
  public static final int MAXIMUM = 255;
  
  /**
   * Minimum value from IR-sensor
   */
  public static final int MINIMUM = 0;
  
  /**
   * Measure the reflex values on both sides and update the values.
   */
  public static native void update();

  /**
   * Get the difference between the measurement with and without IR-enlightment
   * from the left line sensor.
   * @return measured value
   */
  public static native int getLeftRel();

  /**
   * Get the difference between the measurement with and without IR-enlightment
   * from the right line sensor.
   * @return measured value
   */
  public static native int getRightRel();

  /**
   * Get the measured value from the left line sensor with IR-enlightment.
   * @return measured value
   */
  public static native int getLeftAbs();

  /**
   * Get the measured value from the right line sensor with IR-enlightment.
   * @return measured value
   */
  public static native int getRightAbs();
}


