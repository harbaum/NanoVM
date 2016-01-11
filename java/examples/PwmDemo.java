/*
    PwmDemo.java
 */

import nanovm.avr.*;

class PwmDemo {
  public static void main(String[] args) {
    System.out.println("Pulse width modulation demo");

    // 100Hz timer is default, so set it to 1kHz
    Timer.setPrescaler(Timer.DIV8);  // timer base = 1/8 system clock
    Timer.setSpeed(1000);    // timer = 1/1000 timer base -> 1000 Hz

    // this demo uses pwm1 which is always placed on a pin named OC2.
    // pwm0 would be on a pin named OC0 which doesn't exist on the 
    // Mega8 cpu this demo is tested on
    AVR.pwm1.setPrescaler(Pwm.DIV1024);  // 8 Khz
 
    // permanently flash led using PWM1
    while(true) {
      // led is connected to Vcc -> pwm set to 0 will light it 100%
      System.out.println("Going bright to dark");
      for(int i=0;i<256;i++) {
	AVR.pwm1.setRatio(i);
	Timer.wait(2); // wait 2ms
      }

      Timer.wait(500); // wait 500ms

      System.out.println("Going dark to bright");
      for(int i=255;i>=0;i--) {
	AVR.pwm1.setRatio(i);
	Timer.wait(2); // wait 2ms
      }
    }
  }
}

     
