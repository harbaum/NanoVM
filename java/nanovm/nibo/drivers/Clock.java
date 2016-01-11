package nanovm.nibo.drivers;

/**
 * Encapsulates time information and delay methods.
 * @author Nils Springob
 */
public class Clock {

  /**
   * report seconds since system start
   * @return seconds
   */
  public static native int getSeconds();
  
  /**
   * report milliseconds since last second
   * @return milliseconds
   */
  public static native int getMilliSeconds();
  
  /**
   * wait given time in milliseconds
   * @param ms milliseconds
   */
  public static native void delayMilliseconds(int ms);
  
  /**
   * wait given time in microseconds
   * @param us microseconds
   */
  public static native void delayMicroseconds(int us);
}


