/*
  MotorTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class MotorTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Motor Test");
    WheelEncoder.setEnabled(true);
    DistanceSensor.setEnabled(true);

    int val=0;
    int inc=1;

    while(true) {
      DistanceSensor.updateLeft();
      DistanceSensor.updateRight();
      
      Motor.setLeft(val);
      Motor.setRight(val);

      Display.gotoXY(0, 1);
      Display.print("Val="+val+" ");

      Display.gotoXY(0, 2);
      Display.print("enc l="+WheelEncoder.getLeft() + " r="+WheelEncoder.getRight()+"  ");

      Display.gotoXY(0, 3);
      Display.print("dist l="+DistanceSensor.getLeft() + " r="+DistanceSensor.getRight()+"  ");
      val+=inc;
      if (val>500){
        val=500;
        inc=-1;
        Leds.set(Leds.RED);
      }
      if (val<-500){
        val=-500;
        inc=1;
        Leds.set(Leds.WHITE);
      }
      Clock.delayMilliseconds(5);
    }
  }
}

     
