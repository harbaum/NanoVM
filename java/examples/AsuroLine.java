/*
  AsuroLine.java

  (c) 2005 Till Harbaum <till@harbaum.org>

  Simple java program for the asure that makes it follow a black line 
  and turn 180 deg whenever it drives into an obstacle

  Determines the brightness limit by reading the start brightness
  and assuming that this is the light color (not on track).

*/

import nanovm.asuro.*;

class AsuroLine {

  public static void main(String[] args) {
    final int SPEED = 160;
    final int TURN_SPEED = 130;
    int limit;
    int left, right;

    System.out.println("Asuro line follower with obstacle test");

    Asuro.lineLED(Asuro.ON);

    limit = 
      (Asuro.lineSensor(Asuro.LEFT) +
       Asuro.lineSensor(Asuro.RIGHT)) / 4;

    while(true) {
      // did we smash into something??
      if(Asuro.getSwitches(Asuro.ANY) != 0) {
	Asuro.statusLED(Asuro.RED);

	// back off half a second, ...
	Asuro.motor(-SPEED, -SPEED);
	Asuro.wait(500);

	// ... turn until line is lost and ...
	Asuro.motor(TURN_SPEED, -TURN_SPEED);
	do {
	  left  = Asuro.lineSensor(Asuro.LEFT);
	  right = Asuro.lineSensor(Asuro.RIGHT);
	} while((left < limit) || (right < limit));

	// ... continue until the line is found again
	do {
	  left  = Asuro.lineSensor(Asuro.LEFT);
	  right = Asuro.lineSensor(Asuro.RIGHT);
	} while((left >= limit) || (right >= limit));

      } else {
	Asuro.statusLED(Asuro.GREEN);

	left  = Asuro.lineSensor(Asuro.LEFT);
	right = Asuro.lineSensor(Asuro.RIGHT);

	// both LEDs dark -> on track!
	if((left < limit) && (right < limit)) {
	  Asuro.backLED(Asuro.OFF, Asuro.OFF);
	  Asuro.motor(SPEED, SPEED);
	}
	
	// off the track to the left -> turn right
	if((left >= limit) && (right < limit)) {
	  Asuro.backLED(Asuro.ON, Asuro.OFF);
	  Asuro.motor(SPEED, 0);
	}
	
	// off the track to the right -> turn left
	if((left < limit) && (right >= limit)) {
	  Asuro.backLED(Asuro.OFF, Asuro.ON);
	  Asuro.motor(0, SPEED);
	}
	
	// completely of the track ->turn on the spot
	if((left >= limit) && (right >= limit)) {
	  Asuro.backLED(Asuro.ON, Asuro.ON);
	  Asuro.motor(-TURN_SPEED, TURN_SPEED);
	}
      }
    }
  }
}

     
