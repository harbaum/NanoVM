/*
  DistTest.java

  (c) 2007 Nils Springob <nils@nicai-systems.de>
*/

import nanovm.nibo.drivers.*;
import nanovm.util.Formatter;

class DistTest {

  public static void handleSensor(int sensor, int led){
    int dist = DistanceSensor.getSensor(sensor);
    if (led!=Leds.FRONT_RIGHT) {
      GraphicDisplay.print(Formatter.format(dist, " %3d"));
    }
    if (dist<30) {
      Leds.setStatus(led, Leds.GREEN);
    } else if (dist<90) {
      Leds.setStatus(led, Leds.OFF);
    } else if (dist<150) {
      Leds.setStatus(led, Leds.ORANGE);
    } else {
      Leds.setStatus(led, Leds.RED);
    }
  }

  public static void main(String[] args) {
    GraphicDisplay.clear();
    GraphicDisplay.gotoXY(20,10);
    GraphicDisplay.setProportional(true);
    GraphicDisplay.print("Distancesensor Test");
    GraphicDisplay.setProportional(false);
      
    DistanceSensor.setEnabled(true);

    while(true) {
      DistanceSensor.update();
      
      GraphicDisplay.gotoXY(0,40);
            
      handleSensor(DistanceSensor.RIGHT,       Leds.RIGHT);
      handleSensor(DistanceSensor.RIGHT_FRONT, Leds.RIGHT_FRONT);
      handleSensor(DistanceSensor.FRONT,       Leds.FRONT_LEFT);
      handleSensor(DistanceSensor.FRONT,       Leds.FRONT_RIGHT);
      handleSensor(DistanceSensor.LEFT_FRONT,  Leds.LEFT_FRONT);
      handleSensor(DistanceSensor.LEFT,        Leds.LEFT);

      Clock.delayMilliseconds(100);
    }
  }
}

     
