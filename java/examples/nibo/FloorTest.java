/*
  DistTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;
import nanovm.util.Formatter;

class FloorTest {

  public static void main(String[] args) {
    GraphicDisplay.clear();
    GraphicDisplay.gotoXY(20,10);
    GraphicDisplay.setProportional(true);
    GraphicDisplay.print("Edge and Linesensor Test");
    GraphicDisplay.setProportional(false);
      
    while(true) {
      LineDetector.update();
      EdgeDetector.update();
      
      GraphicDisplay.gotoXY(0,40);
      GraphicDisplay.print(Formatter.format(EdgeDetector.getRightRel(), " %+4d"));
      GraphicDisplay.print(Formatter.format(LineDetector.getRightRel(), " %+4d"));
      GraphicDisplay.print(Formatter.format(LineDetector.getLeftRel(), " %+4d"));
      GraphicDisplay.print(Formatter.format(EdgeDetector.getLeftRel(), " %+4d"));
      
      GraphicDisplay.gotoXY(0,50);
      GraphicDisplay.print(Formatter.format(EdgeDetector.getRightAbs(), " %4d"));
      GraphicDisplay.print(Formatter.format(LineDetector.getRightAbs(), " %4d"));
      GraphicDisplay.print(Formatter.format(LineDetector.getLeftAbs(), " %4d"));
      GraphicDisplay.print(Formatter.format(EdgeDetector.getLeftAbs(), " %4d"));

      Clock.delayMilliseconds(100);
    }
  }
}

     
