/*
  MouseTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class MouseTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Mouse Test");

    int x=0;
    int y=0;
    int last_qual=0;
    int i=0;

    while(true) {
      i++;
      int dx = Mouse.getX();
      int dy = Mouse.getY();
      if (dx!=0 || dy!=0) {
        x+=dx;
        y+=dy;
        Display.gotoXY(0, 2);
        Display.print("  x=" + x + " y=" + y + "  ");
      }
      int qual = Mouse.getQuality();
      if (qual!=last_qual) {
        last_qual = qual;
        Display.gotoXY(0, 1);
        Display.print("Qual="+qual+"  ");
      }
      if (i==100){
        Leds.set(Leds.BLUE);
      } else if (i==200){
        Leds.set(Leds.WHITE);
        i=0;
      }
    }
  }
}

     
