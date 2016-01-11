/*
  AsuroMotor.java

  (c) 2005 Till Harbaum <till@harbaum.org>
*/

import nanovm.asuro.*;

class AsuroMotor {

  public static void main(String[] args) {
    int i;

    System.out.println("Asuro motor test");

    while(true) {
      Asuro.statusLED(Asuro.GREEN);
      for(i=0;i<=255;i++) {
	Asuro.motor(0, i);
	Asuro.wait(10);
      }

      for(i=255;i>=-255;i--) {
	Asuro.motor(0, i);
	Asuro.wait(10);
      }

      for(i=-255;i<=0;i++) {
	Asuro.motor(0, i);
	Asuro.wait(10);
      }

      Asuro.statusLED(Asuro.RED);
      for(i=0;i<=255;i++) {
	Asuro.motor(i, 0);
	Asuro.wait(10);
      }

      for(i=255;i>=-255;i--) {
	Asuro.motor(i, 0);
	Asuro.wait(10);
      }

      for(i=-255;i<=0;i++) {
	Asuro.motor(i, 0);
	Asuro.wait(10);
      }

    }
  }
}

     
