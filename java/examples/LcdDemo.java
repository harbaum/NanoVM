/*
  LcdDemo.java
 */

import nanovm.io.*;
import nanovm.avr.*;

class LcdDemo {
  public static void main(String[] args) {
    int rnd = 0;

    System.out.println("LCD test");

    // clear lcd
    Lcd.clear();

    // write something to the first line
    Lcd.gotoYX(0, 0);
    Lcd.print("Hallo Java-Welt!");

    // and something to the second line
    Lcd.gotoYX(1, 0);
    Lcd.print("Rnd xxxxx Cnt xxxxx");

    // forever ...
    while(true) {
      // clear positions used for digits
      Lcd.gotoYX(1, 4);
      Lcd.print("     ");
      Lcd.gotoYX(1, 14);
      Lcd.print("     ");

      Lcd.gotoYX(1, 4);
      Lcd.print(rnd++);

      // count from 0 to 12000 (20 min)
      for(int cnt=0;cnt<12000;cnt++) {
	Lcd.gotoYX(1, 14);
	Lcd.print(cnt);

	// wait 100ms (10 ticks at 100Hz)
	Timer.wait(10);
      }
    }
  }
}
