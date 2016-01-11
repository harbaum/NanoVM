package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;
import ctbot.utils.Navigator;


public class NaviGoalFinePos extends NaviGoal {
  private float targetX;
  private float targetY;
  private float targetDelta;

  public NaviGoalFinePos(float x, float y, float delta) {
    targetX = x;
    targetY = y;
    targetDelta = delta;
    this.subGoal = null;
    this.nextGoal = null;
  }

  public boolean work(){ 
    float dx = targetX-Odometry.x;
    float dy = targetY-Odometry.y;
    float dori = Odometry.normalizeOri(Math.atan2(dx, dy)-Odometry.ori);
    float dist = Math.sqrt(dx*dx+dy*dy);
    if (dist<targetDelta)
      return false;

    float gamma = Math.PI-Math.abs(dori);
    float alpha = Math.abs(Odometry.normalizeOri(Odometry.ori-Math.atan2(dx, dy)));
    float betha = Math.PI-gamma-alpha;

    if (alpha<betha){
        Navigator.goStraight(Navigator.MAX_SPEED);
    } else {
      float radius = dist * Math.sin(betha)/Math.sin(gamma)*Math.tan(0.5f*gamma);
      if (alpha<0.05f){
        Navigator.goStraight(Navigator.MAX_SPEED);
      } else if (dori>0.0f) {
        Navigator.goCircle(radius, Navigator.MAX_SPEED);
      } else if (dori<0.0f) {
        Navigator.goCircle(-radius, Navigator.MAX_SPEED);
      } else {
        Navigator.goStraight(Navigator.MAX_SPEED);
      }
    }

    return true; // Goal needs additional work...
  }
}
