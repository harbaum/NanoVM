package nanovm.nibo.drivers;

/**
 * Provides methods for the motor controll. All speed information has to be
 * given in ticks, a tick is 1/20 of a whole rotation.
 * see <a href="http://nibo.editthis.info/wiki/Motorcontroller-Firmware">Nibo Wiki</a> for details.
 * @author Nils Springob
 */
public class Motor {
  public static final int MAXIMUM_PWM = 1024;
  public static final int MINIMUM_PWM = -1024;
  public static final int MAXIMUM_SPEED = 200;
  public static final int MINIMUM_SPEED = -200;

  /**
   * Set the parameters for the motor control algorithm.
   * @param p proportional factor
   * @param i integral factor
   * @param d differential factor
   */
  public static native void setParameters(int p, int i, int d);
  
  /**
   * Bypass the motor controller and set the PWM values directly.
   * @param left pwm value for the left motor
   * @param right pwm value for the right motor
   */
  public static native void setPWM(int left, int right);
  
  /**
   * Set the target speed for the motor controller.
   * @param left target speed for the left motor in ticks per second
   * @param right target speed for the right motor in ticks per second
   */
  public static native void setSpeed(int left, int right);

  /**
   * Stop all motor activity immediatly.
   */
  public static native void stop();
  
}

