/*
  AsuroLED.java

  testing asuros status led by cycling through all
  four possible states and pausing a second in between

  (c) 2005 Till Harbaum <till@harbaum.org>
*/

import nanovm.asuro.*;

class AsuroLED {

  // set status led state and wait a second
  static void led(int state) {
    Asuro.statusLED(state);
    Asuro.wait(1000);    
  }

  public static void main(String[] args) {
    System.out.println("Asuro LED test");

    while(true) {
      // test status led
      led(Asuro.GREEN);
      led(Asuro.YELLOW);
      led(Asuro.RED);
      led(Asuro.OFF);

      // test back leds
      Asuro.backLED(Asuro.ON, Asuro.OFF);
      Asuro.wait(1000);    
      Asuro.backLED(Asuro.OFF, Asuro.ON);
      Asuro.wait(1000);    
      Asuro.backLED(Asuro.OFF, Asuro.OFF);

      // test line led
      Asuro.lineLED(Asuro.ON);
      Asuro.wait(1000);    
      Asuro.lineLED(Asuro.OFF);
    }
  }
}

     
