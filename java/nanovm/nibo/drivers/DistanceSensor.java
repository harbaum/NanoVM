package nanovm.nibo.drivers;

/**
 * Encapsulates the information about the five
 * IR-reflex-sensors. You will have to call the update Method first to get
 * actual values.
 * @author Nils Springob
 */
public class DistanceSensor {

  /**
   * Maximum value from IR-sensor
   */
  public static final int MAXIMUM = 255;
  
  /**
   * Minimum value from IR-sensor
   */
  public static final int MINIMUM = 0;
  
  /**
   * ID of the left IR-sensor
   */
  public static final int LEFT        = 4;
  
  /**
   * ID of the left front IR-sensor
   */
  public static final int LEFT_FRONT  = 3;
  
  /**
   * ID of the front IR-sensor
   */
  public static final int FRONT       = 2;
  
  /**
   * ID of the right front IR-sensor
   */
  public static final int RIGHT_FRONT = 1;
  
  /**
   * ID of the right IR-sensor
   */
  public static final int RIGHT       = 0;
  
  /**
   * update all values
   */
  public static native void update();
  
  /**
   * get the value of the IR-reflex-sensor with the given ID
   * @param sensor ID of the IR-sensor
   * @return value of the IR-sensor
   */
  public static native int getSensor(int sensor);

  /**
   * enable and disable the IR-reflex-sensors
   * @param enable true to enable sensors
   */
  public static native void setEnabled(boolean enable);

  /**
   * get status of IR-reflex-sensors
   * @return true when sensors are enabled
   */
  public static native boolean getEnabled();
  
}


