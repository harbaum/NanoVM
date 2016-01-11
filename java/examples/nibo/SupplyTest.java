/*
  SupplyTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;
import nanovm.util.Formatter;

class SupplyTest {

  public static void main(String[] args) {

    GraphicDisplay.clear();
    GraphicDisplay.setProportional(true);
    GraphicDisplay.gotoXY(40, 10);
    GraphicDisplay.print("Supply test");
    
    GraphicDisplay.gotoXY(5, 30);
    GraphicDisplay.print("Supply Voltage:");
    GraphicDisplay.setProportional(false);
      
    while(true) {
      Bot.update();
      int supply = Bot.getSupplyVoltage();
      float fsupp = 0.016565F*supply-1.19F;
      GraphicDisplay.gotoXY(80, 30);
      GraphicDisplay.print(Formatter.format(supply, "%5i"));
      GraphicDisplay.gotoXY(80, 40);
      GraphicDisplay.print(Formatter.format(fsupp, "%5.2f V"));
      Clock.delayMilliseconds(1);
    }
  }
}

     
