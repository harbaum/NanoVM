/*
  Test.java

  testing ctbot leds by cycling through the six
  status leds and blinking all blue leds

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class Test {

  // set status led state and wait a second
  static void led(int state, int ms) {
    Leds.set(state);
    Display.gotoYX(2, 1);
    Display.print("Ledval="+state+"  ");
    Clock.delayMilliseconds(ms);
  }

  public static void main(String[] args) {
    led(255,200);
    led(0,1000);
    Display.clear();
    Display.gotoYX(1, 1);
    Display.print("Test!");


    while(true) {
      // test leds
      led(Leds.RED,200);
      led(Leds.ORANGE,200);
      led(Leds.YELLOW,200);
      led(Leds.GREEN,200);
      led(Leds.BLUE,200);
      led(Leds.WHITE,200);
      led(0,1000);
      led(Leds.BLUE_LEFT+Leds.BLUE+Leds.BLUE_RIGHT,50);
      led(0,500);
      led(Leds.BLUE_LEFT+Leds.BLUE+Leds.BLUE_RIGHT,50);
      led(0,500);
      led(Leds.BLUE_LEFT+Leds.BLUE+Leds.BLUE_RIGHT,50);
      led(0,500);
      led(Leds.BLUE_LEFT+Leds.BLUE+Leds.BLUE_RIGHT,50);
      led(0,1000);
    }
  }
}

     
