/*
  IrTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class IrTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Ir Test");

    int val=0;
    int inc=1;
    int lastcmd=0;
    int count=0;

    while(true) {
      int cmd = IrReceiver.getCommand();
      if (cmd!=0)
      {
        if (cmd!=lastcmd)
          count=1;
        else
          count++;
        lastcmd=cmd;
        boolean toggle = ((cmd&0x800)!=0);
        int device = (cmd&0x7c0)/0x40;
        cmd %= 0x40;
      
        Leds.set(Leds.BLUE+Leds.WHITE);
        Display.gotoXY(2, 1);
        Display.print("counter = "+count+"   ");
        Display.gotoXY(2, 2);
        Display.print("device  = "+device+"   ");
        Display.gotoXY(2, 3);
        Display.print("command = "+cmd+"   ");
        Display.gotoXY(10, 0);
        Display.print(toggle?"*":" ");
        Clock.delayMilliseconds(10);
        Leds.set(Leds.WHITE);
      }
    }
  }
}

     
