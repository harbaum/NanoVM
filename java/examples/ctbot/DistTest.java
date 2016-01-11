/*
  DistTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;
import nanovm.util.Formatter;
import ctbot.utils.FreeDist;

class DistTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Dist Test");

    while(true) {
      FreeDist.update();
      float distL = FreeDist.left;
      float distR = FreeDist.right;
      
      int rawL = DistanceSensor.getLeft();
      int rawR = DistanceSensor.getRight();
      
      Display.gotoXY(0, 1);
      Display.print(Formatter.format(rawL, "raw  L=%4d "));
      Display.print(Formatter.format(rawR,     " R=%4d "));
      Display.gotoXY(0, 2);
      Display.print(Formatter.format(distL, "dist L=%4.2f "));
      Display.print(Formatter.format(distR,     " R=%4.2f "));
      Clock.delayMilliseconds(20);
    }
  }
}

     
