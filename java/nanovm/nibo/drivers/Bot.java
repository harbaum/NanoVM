package nanovm.nibo.drivers;

/**
 * Gives access to general information about the Nibo robot.
 * You will have to call update first to get actual values.
 * @author Nils Springob
 */
public class Bot {
  
  /**
   * update bot data
   */
  public static native void update();

  /**
   * get the supply voltage.
   * @return ticks for supply voltage
   */
  public static native int getSupplyVoltage();
}

