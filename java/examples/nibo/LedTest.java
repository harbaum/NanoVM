/*
  LedTest.java
  
  test nibo leds by fading front-leds
  and blinking status-leds

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;

class LedTest {

  // set status led state and wait a second
  static void led(int led, int color, int ms) {
    Leds.setStatus(led, color);
    Clock.delayMilliseconds(ms);
  }
  
  // set status led state and wait a second
  static void frontleds(int light, int ms) {
    Leds.setHeadlights(light);
    Clock.delayMilliseconds(ms);
  }

  public static void main(String[] args) {

    for (int i=0; i<1024; i++) {
      frontleds(i, 2);
    }
    
    for (int i=1024; i>=0; i--) {
      frontleds(i, 1);
    }

    while(true) {
      // test leds
      led(Leds.LEFT,        Leds.GREEN, 200);
      led(Leds.LEFT_FRONT,  Leds.GREEN, 200);
      led(Leds.FRONT_LEFT,  Leds.GREEN, 200);
      led(Leds.FRONT_RIGHT, Leds.GREEN, 200);
      led(Leds.RIGHT_FRONT, Leds.GREEN, 200);
      led(Leds.RIGHT,       Leds.GREEN, 200);
      
      led(Leds.LEFT,        Leds.ORANGE, 200);
      led(Leds.LEFT_FRONT,  Leds.ORANGE, 200);
      led(Leds.FRONT_LEFT,  Leds.ORANGE, 200);
      led(Leds.FRONT_RIGHT, Leds.ORANGE, 200);
      led(Leds.RIGHT_FRONT, Leds.ORANGE, 200);
      led(Leds.RIGHT,       Leds.ORANGE, 200);
    
      led(Leds.LEFT,        Leds.RED, 200);
      led(Leds.LEFT_FRONT,  Leds.RED, 200);
      led(Leds.FRONT_LEFT,  Leds.RED, 200);
      led(Leds.FRONT_RIGHT, Leds.RED, 200);
      led(Leds.RIGHT_FRONT, Leds.RED, 200);
      led(Leds.RIGHT,       Leds.RED, 200);
    
      led(Leds.LEFT,        Leds.OFF, 200);
      led(Leds.LEFT_FRONT,  Leds.OFF, 200);
      led(Leds.FRONT_LEFT,  Leds.OFF, 200);
      led(Leds.FRONT_RIGHT, Leds.OFF, 200);
      led(Leds.RIGHT_FRONT, Leds.OFF, 200);
      led(Leds.RIGHT,       Leds.OFF, 200);
    }
  }
}

     
