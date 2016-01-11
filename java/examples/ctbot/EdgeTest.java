/*
  EdgeTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class EdgeTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Edge Test");
    EdgeDetector.setEnabled(false);

    int val=0;
    int inc=1;

    while(true) {
      EdgeDetector.updateLeft();
      EdgeDetector.updateRight();
      int l0 = EdgeDetector.getLeft();
      int r0 = EdgeDetector.getRight();
      
      EdgeDetector.setEnabled(true);
      Clock.delayMicroseconds(100);
      EdgeDetector.updateLeft();
      EdgeDetector.updateRight();
      int l1 = EdgeDetector.getLeft();
      int r1 = EdgeDetector.getRight();
      EdgeDetector.setEnabled(false);

      Display.gotoXY(0, 1);
      Display.print(" l="+l1+"  ");
      
      Display.gotoXY(10, 1);
      Display.print(" r="+r1+"  ");
      
      Display.gotoXY(0, 2);
      Display.print("dl="+(l1-l0)+"  ");
      
      Display.gotoXY(10, 2);
      Display.print("dr="+(r1-r0)+"  ");
      
      Display.gotoXY(0, 3);
      Display.print((l1-l0<-20)?"*****":"     ");
      
      Display.gotoXY(10, 3);
      Display.print((r1-r0<-20)?"*****":"     ");
      
      Clock.delayMilliseconds(20);
    }
  }
}

     
