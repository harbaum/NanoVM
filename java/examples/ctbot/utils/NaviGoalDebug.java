package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;
import ctbot.utils.Navigator;

// subgoal for NaviGoalRoughlyPos
// will locate some space, and insterts a new NaviGoalRoughlyPos as nextGoal
public class NaviGoalDebug extends NaviGoal {
  private int counter;
  private int ledColor;

  public NaviGoalDebug(int counter, int ledColor) {
    this.counter = counter;
    this.ledColor = ledColor;
  }

  public boolean work(){
    Navigator.stop();
    if (counter==0)
      return false;
    Leds.set(ledColor);
    counter--;
    return true; // Goal needs additional work...
  }

}
