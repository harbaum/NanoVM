package nanovm.nibo.drivers;

/**
 * Provides methods to send and receive RC5-IR-codes.
 * @author Nils Springob
 */
public class IrTransceiver {

  /**
   * Get last received command
   * @return received RC5 code, 0 for no command
   */
  public static native int getCommand();
  
  /**
   * Transmitt command
   * @param command RC5 code to transmitt
   */
  public static native int sendCommand(int command);
}

