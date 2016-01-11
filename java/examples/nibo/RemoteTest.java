/*
  RemoteTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;
import nanovm.util.Formatter;

class RemoteTest {
  private static final int IR_DEV = 0x0040;
  private static final int IR_MASK = 0x07ff;

  private static final int IR_SPEED_INC = 16*IR_DEV + 16;
  private static final int IR_SPEED_DEC = 16*IR_DEV + 17;
  private static final int IR_DIR_LEFT  =  0*IR_DEV + 17;
  private static final int IR_DIR_RIGHT =  0*IR_DEV + 16;
  private static final int IR_POW_INC   =  0*IR_DEV + 32;
  private static final int IR_POW_DEC   =  0*IR_DEV + 33;

  public static void main(String[] args) {
    GraphicDisplay.clear();
    GraphicDisplay.gotoXY(20,10);
    GraphicDisplay.setProportional(true);
    GraphicDisplay.print("Remote control test");
    GraphicDisplay.setProportional(false);

    int lSpeed = 0;
    int rSpeed = 0;
    int lTicks = 0;
    int rTicks = 0;

    while(true) {
      int cmd = IrTransceiver.getCommand();

      if (cmd!=0) {
        GraphicDisplay.gotoXY(50,30);
        GraphicDisplay.print(Formatter.format(cmd, " %4x"));

        cmd&=IR_MASK;
        switch (cmd) {
          case IR_SPEED_INC: lSpeed=+100; rSpeed=+100; break;
          case IR_SPEED_DEC: lSpeed=-100; rSpeed=-100; break;
          case IR_DIR_LEFT:  lSpeed=-20; rSpeed=+20; break;
          case IR_DIR_RIGHT: lSpeed=+20; rSpeed=-20; break;
          case IR_POW_INC: break;
          case IR_POW_DEC: break;
          default: lSpeed=0; rSpeed=0; break;
        
        }
        GraphicDisplay.gotoXY( 0,30);
        GraphicDisplay.print(Formatter.format(rSpeed, " %+5i"));
        GraphicDisplay.gotoXY(90,30);
        GraphicDisplay.print(Formatter.format(lSpeed, " %+5i"));
        Motor.setSpeed(lSpeed, rSpeed);
        
      }
      WheelEncoder.update();
      rTicks += WheelEncoder.getRightInc();
      lTicks += WheelEncoder.getLeftInc();
      GraphicDisplay.gotoXY( 0, 50);
      GraphicDisplay.print(Formatter.format(rTicks, " %+5i"));
      GraphicDisplay.gotoXY(90, 50);
      GraphicDisplay.print(Formatter.format(lTicks, " %+5i"));

      Clock.delayMilliseconds(100);
    }
  }
}

     
