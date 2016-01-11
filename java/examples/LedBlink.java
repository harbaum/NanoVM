/*
  LedBlink.java
 */

import nanovm.avr.*;

class LedBlink {
  public static void main(String[] args) {
    System.out.println("Blinking LED");

    System.out.println("System clock = " + AVR.getClock() + "000 Hz");

    // both led ports are outputs
    AVR.portB.setOutput(0);
    AVR.portB.setOutput(1);
    
    // no need to setup timer, since 100Hz is default ...

    while(true) {
      // PortB.0/1 for Demoboard
      AVR.portB.setBit(1);
      AVR.portB.setBit(0);
      Timer.wait(100);
      AVR.portB.clrBit(0);
      Timer.wait(100);

      AVR.portB.clrBit(1);
      AVR.portB.setBit(0);
      Timer.wait(100);
      AVR.portB.clrBit(0);
      Timer.wait(100);
    }
  }
}
