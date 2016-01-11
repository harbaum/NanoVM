/*
  MotorTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.ctbot.drivers.*;

class MotorSpeedTest {

  public static void main(String[] args) {

    Leds.set(Leds.WHITE);
    Display.clear();
    Display.gotoXY(0, 0);
    Display.print("Motor Test");
    WheelEncoder.setEnabled(true);
    DistanceSensor.setEnabled(true);

    int val=0;
    int speed=0;

    while(true) {
      DistanceSensor.updateLeft();
      DistanceSensor.updateRight();
      
      Motor.setLeftSpeed(speed);
      Motor.setRightSpeed(speed);

      Display.gotoXY(0, 1);
      Display.print("speed = "+speed+"  ");

      Display.gotoXY(0, 2);
      Display.print("speed l="+WheelEncoder.getLeftSpeed() + " r="+WheelEncoder.getRightSpeed()+"  ");

      Display.gotoXY(0, 3);
      Display.print("mtr l="+Motor.getLeft() + " r="+Motor.getRight()+"  ");
      
      if (val==0)
      {
        speed=10;
      }
      if (val==200)
      {
        speed=20;
      }
      if (val==400)
      {
        speed=40;
      }
      if (val==500)
      {
        speed=80;
      }
      if (val==600)
      {
        speed=120;
      }

      val++;
      if (val>700){
        val=0;
      }
      Clock.delayMilliseconds(5);
    }
  }
}

     
