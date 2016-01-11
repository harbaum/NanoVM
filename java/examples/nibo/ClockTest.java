/*
  ClockTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;
import nanovm.util.Formatter;

class ClockTest {

  public static void main(String[] args) {
    GraphicDisplay.clear();
    GraphicDisplay.setProportional(true);
    GraphicDisplay.gotoXY(40, 10);
    GraphicDisplay.print("Clock test");
    
    GraphicDisplay.gotoXY(5, 30);
    GraphicDisplay.print("System time:");
    GraphicDisplay.setProportional(false);
    
    while(true) {
      GraphicDisplay.gotoXY(70, 30);
      GraphicDisplay.print(Formatter.format(Clock.getSeconds(), "%5i."));
      GraphicDisplay.print(Formatter.format(Clock.getMilliSeconds(), "%3i"));
      Clock.delayMilliseconds(1);
    }
  }
}

     
