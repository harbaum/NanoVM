/*
  IrTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import nanovm.util.Formatter;
import ctbot.utils.*;

class NavigatorTest {

  private static final int IR_DEV = 0x0040;
  private static final int IR_MASK = 0x07ff;
  
  private static final int IR_COMAMND_0 = 0*IR_DEV + 0;
  private static final int IR_COMAMND_1 = 0*IR_DEV + 1;
  private static final int IR_COMAMND_2 = 0*IR_DEV + 2;
  private static final int IR_COMAMND_3 = 0*IR_DEV + 3;
  private static final int IR_COMAMND_4 = 0*IR_DEV + 4;
  private static final int IR_COMAMND_5 = 0*IR_DEV + 5;


  public static void updatePos(){
    Odometry.update();
    Display.gotoXY(0,1);
    Display.print(Formatter.format(Odometry.x, "x=%+8.3f"));
    Display.gotoXY(0,2);
    Display.print(Formatter.format(Odometry.y, "y=%+8.3f"));
    Display.gotoXY(0,3);
    Display.print(Formatter.format(Math.toDegrees(Odometry.ori), "ori=%+4.0f"));
    
    Display.gotoXY(12,1);
    Display.print(Formatter.format(FreeDist.left, "l=%4.2f  "));
    Display.gotoXY(12,2);
    Display.print(Formatter.format(FreeDist.right, "r=%4.2f  "));
  }

  public static void execute(){
    updatePos();
    Leds.set(Leds.WHITE);
    if (Navigator.work()){
      if (Navigator.nextGoal!=null) {
        if (Navigator.nextGoal.subGoal!=null)
          Leds.set(Leds.YELLOW);
        else if (Navigator.nextGoal.nextGoal!=null)
          Leds.set(Leds.ORANGE);
        else
          Leds.set(Leds.RED);
      } else
        Leds.set(Leds.WHITE);
    }else{
      Leds.set(Leds.GREEN);
    }
  }
  
  public static void main(String[] args) {
  
    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("DriverTest");
    DistanceSensor.setEnabled(true);
    int lastcmd = 0;

    while(true) {
      int cmd = IrReceiver.getCommand();
      if ((cmd!=0) & (cmd!=lastcmd)) {
        lastcmd=cmd;
        switch(cmd & IR_MASK) {
          case IR_COMAMND_0:
            Display.gotoXY(10, 0);
            Display.print("STOP");
            while(Navigator.popGoal()!=null);
              
            break;
            
          case IR_COMAMND_1:
            Display.gotoXY(10, 0);
            Display.print("HOME");
            Navigator.appendGoal(new NaviGoalRoughlyPos(0.0f, 0.0f, 0.05f, false));
            Navigator.appendGoal(new NaviGoalFinePos(0.0f, 0.0f, 0.05f));
            Navigator.appendGoal(new NaviGoalOri(0.0f, 0.04f));
            break;
            
          case IR_COMAMND_2:
            Display.gotoXY(10, 0);
            Display.print("BACK");
            Navigator.appendGoal(new NaviGoalOri(Math.PI, 0.04f));
            break;
            
          case IR_COMAMND_3:
            Display.gotoXY(10, 0);
            Display.print("POS3");
            Navigator.appendGoal(new NaviGoalRoughlyPos(0.2f, 0.2f, 0.05f, false));
            break;
            
          case IR_COMAMND_4:
            Display.gotoXY(10, 0);
            Display.print("POS4");
            Navigator.appendGoal(new NaviGoalRoughlyPos(0.4f, 0.1f, 0.05f, false));
            break;
            
          case IR_COMAMND_5:
            Display.gotoXY(10, 0);
            Display.print("POS5");
            Navigator.appendGoal(new NaviGoalRoughlyPos(1.0f, 0.0f, 0.05f, false));
            break;
            
          default:
            Display.gotoXY(10, 0);
            Display.print("ir="+(cmd & IR_MASK));
        }
      }
      execute();
    }
  }
}


