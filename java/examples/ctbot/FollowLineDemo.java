/*
  FollowLineDemo.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import nanovm.util.Formatter;

class FollowLineDemo {

  static final float M_PER_TICK   = 0.00274f;
  static final float M_WHEEL_DIST = 0.10000f;

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Follow Line");

    int lastL=0;
    int lastR=0;
    int count=0;

    // initial position and orientation
    float head = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float s = 0.0f;

    while(true) {
      // update position
      int dsl = WheelEncoder.getLeftInc();
      int dsr = WheelEncoder.getRightInc();
      float dh = (dsl-dsr)*(M_PER_TICK/M_WHEEL_DIST);
      head += dh;
      if (head>Math.PI)
        head-=2.0f*Math.PI;
      if (head<-Math.PI)
        head+=2.0f*Math.PI;
      float ds = (dsl+dsr) * (0.5f * M_PER_TICK);
      s += Math.abs(ds);
      x += ds * Math.cos(head);
      y += ds * Math.sin(head);

      LineDetector.setEnabled(true);
      Clock.delayMicroseconds(100);
      LineDetector.updateLeft();
      LineDetector.updateRight();
      int dl = LineDetector.getLeft();
      int dr = LineDetector.getRight();
      LineDetector.setEnabled(false);

      int delta = dl-dr;
      int sum = dl+dr;
      int min = (dl<dr)?dl:dr;

      if (sum>180)
      {
        min/=40;
        delta/=40;
        if (min<0)
          min=0;

        int l = (min - delta);
        int r = (min + delta);

        Motor.setLeftSpeed(l);
        Motor.setRightSpeed(r);
        lastL=l;
        lastR=r;
        count=0;
        Display.gotoXY(15, 0);
        Display.print("    ");

        Leds.set(Leds.GREEN | ((l<0)?Leds.BLUE_LEFT:0) | ((r<0)?Leds.BLUE_RIGHT:0));

      }
      else
      {
        if (count++==0)
        {
          if (lastL>lastR)
          {
            Motor.setLeftSpeed(+10-4);
            Motor.setRightSpeed(-10-4);
          }
          else
          {
            Motor.setLeftSpeed(-10-4);
            Motor.setRightSpeed(+10-4);
          }
          Display.gotoXY(15, 0);
          Leds.set(Leds.YELLOW);
          Display.print("ESC ");
        }
        if (count>50)
        {
          Motor.setLeft(0);
          Motor.setRight(0);
          Display.gotoXY(15, 0);
          Leds.set(Leds.RED);
          Display.print("TRAP");
        }
      }
      
      Display.gotoXY(0, 1);
      Display.print(Formatter.format(dl,  "l:%4d,"));
      Display.print(Formatter.format(dr, "%4d "));
      Display.gotoXY(0, 2);
      Display.print(Formatter.format(lastL,  "s:%4d,"));
      Display.print(Formatter.format(lastR, "%4d "));
      Display.gotoXY(0, 3);
      Display.print(Formatter.format(x,  "pos=(%+6.3f,"));
      Display.print(Formatter.format(y, "%+6.3f) "));
      Display.gotoXY(12, 2);
      Display.print(Formatter.format(Math.toDegrees(head),  "o=%+4.0f "));
      Display.gotoXY(12, 1);
      Display.print(Formatter.format(s,  "d=%6.2f "));
      
      Clock.delayMilliseconds(20);
    }
  }
}

     
