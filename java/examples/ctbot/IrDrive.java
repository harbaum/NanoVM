/*
  IrTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import nanovm.util.Formatter;

class IrDrive {
  private static final int IR_DEV = 0x0040;
  private static final int IR_MASK = 0x07ff;
  
  private static final int IR_SPEED_INC = 16*IR_DEV + 16;
  private static final int IR_SPEED_DEC = 16*IR_DEV + 17;
  private static final int IR_DIR_LEFT  =  0*IR_DEV + 17;
  private static final int IR_DIR_RIGHT =  0*IR_DEV + 16;
  private static final int IR_POW_INC   =  0*IR_DEV + 32;
  private static final int IR_POW_DEC   =  0*IR_DEV + 33;

  private static final float M_PER_TICK   = 0.0031f;
  private static final float M_WHEEL_DIST = 0.1000f;

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Ir Drive");

    int val=0;
    int inc=1;
    int lastcmd=0;
    int count=0;

    int speedL=0;
    int speedR=0;
    int powL=0;
    int powR=0;

    while(true) {
    
      int cmd = IrReceiver.getCommand();
      if (cmd!=0)
      {
        lastcmd=cmd;

        Leds.set(Leds.BLUE+Leds.WHITE);

        switch(cmd & IR_MASK) {
          case IR_SPEED_INC:
            speedL+=2;
            speedR+=2;
            powL=0;
            powR=0;
            break;

          case IR_SPEED_DEC:
            speedL-=2;
            speedR-=2;
            powL=0;
            powR=0;
            break;

          case IR_DIR_LEFT:
            speedL--;
            speedR++;
            powL=0;
            powR=0;
            break;

          case IR_DIR_RIGHT:
            speedL++;
            speedR--;
            powL=0;
            powR=0;
            break;
          
          case IR_POW_INC:
            speedL=0;
            speedR=0;
            powL++;
            powR++;
            break;

          case IR_POW_DEC:
            speedL=0;
            speedR=0;
            powL--;
            powR--;
            break;

          default:
            Display.gotoXY(0, 2);
            Display.print(Formatter.format((cmd&IR_MASK), "IRcmd = 0x%04x "));
            speedL=0;
            speedR=0;
            powL=0;
            powR=0;
            break;
        }

        if (speedL==0)
          Motor.setLeft(powL);
        else
          Motor.setLeftSpeed(speedL);

        if (speedR==0)
          Motor.setRight(powR);
        else
          Motor.setRightSpeed(speedR);

        Display.gotoXY(0, 1);
        Display.print(Formatter.format(speedL, "speed L=%+4d"));
        Display.print(Formatter.format(speedR,      " R=%+4d"));
        Clock.delayMilliseconds(10);
        Leds.set(Leds.WHITE);
      }
      Clock.delayMilliseconds(10);
    }
  }
}


