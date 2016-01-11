/*
  TimeTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class TimeTest {

  public static void main(String[] args) {
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Time Test");

    int myMsecs = 0;
    int mySecs = 0;
    int actSecs = 0;

    while(true) {
      int secs = Clock.getSeconds();
      if (actSecs!=secs)
      {
        Display.gotoXY(0, 2);
        Display.print("Timer="+secs);
        actSecs=secs;
      }
      int msecs = Clock.getMilliSeconds();
      if (msecs<100)
        Leds.set(Leds.RED);
      else if (msecs<200)
        Leds.set(Leds.ORANGE);
      else
        Leds.set(0);
      Clock.delayMilliseconds(1);
      myMsecs++;
      if (myMsecs==1000)
      {
        mySecs++;
        myMsecs=0;
        Display.gotoXY(0, 3);
        Display.print("CPU="+mySecs);
        Display.gotoXY(10, 3);
        Display.print("idle="+100*mySecs/actSecs+"%");
        
      }
    }
  }
}

     
