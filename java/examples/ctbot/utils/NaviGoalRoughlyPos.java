package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;
import ctbot.utils.Navigator;


public class NaviGoalRoughlyPos extends NaviGoal {
  private float x;
  private float y;
  private float delta;
  private boolean doNotFindSpace;

  public NaviGoalRoughlyPos(float x, float y, float delta, boolean doNotFindSpace) {
    this.x = x;
    this.y = y;
    this.delta = delta;
    this.doNotFindSpace = doNotFindSpace;
    this.subGoal = null;
    this.nextGoal = null;
  }

  public boolean work(){
    float dx = x-Odometry.x;
    float dy = y-Odometry.y;
    float dori = Odometry.normalizeOri(Math.atan2(dx, dy)-Odometry.ori);
    float dist = Math.sqrt(dx*dx+dy*dy);
    if (dist<delta)
      return false;

    float radius = (0.25f-Math.abs(dori)/Math.PI)*4.0f;
    if (Math.abs(dori)<0.5f*Math.PI)
      radius=Math.max(radius, 0.10f);
    else
      radius = 0.01f;

    float space = Math.min(FreeDist.left, FreeDist.right);
    if (space<0.20f){
      if (doNotFindSpace)
        return false;
      float alpha = Odometry.normalizeOri(dori+Odometry.ori+Math.PI*0.33f);
      float betha = Odometry.normalizeOri(dori+Odometry.ori-Math.PI*0.33f);
      
      if (dori>0)
        subGoal=new NaviGoalFindSpace(alpha, betha, Math.PI*0.10f, 0.35f, 0.35f);
      else
        subGoal=new NaviGoalFindSpace(betha, alpha, Math.PI*0.10f, 0.35f, 0.35f);
        
      Navigator.halt();
      return true;
    }

    if (dori>Navigator.RAD_FAR_DELTA) {
      Navigator.goCircle(-radius, Navigator.checkSpeed(Navigator.MAX_SPEED, dist));
    } else if (dori<-Navigator.RAD_FAR_DELTA) {
      Navigator.goCircle(radius, Navigator.checkSpeed(Navigator.MAX_SPEED, dist));
    } else {
      Navigator.goStraight(Navigator.checkSpeed(Navigator.MAX_SPEED, dist));
    }
    return true; // Goal needs additional work...
  }
  
}
