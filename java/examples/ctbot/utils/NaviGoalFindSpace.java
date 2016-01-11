package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;
import ctbot.utils.Navigator;

// subgoal for NaviGoalRoughlyPos
// will locate some space, and insterts a new NaviGoalRoughlyPos as nextGoal
public class NaviGoalFindSpace extends NaviGoal {
  private float alpha;
  private float betha;
  private float rho;
  private float minDist;
  private float targetDist;
  private int mode;
  private static final int MODE_FIRST_CLOCKWISE=0;
  private static final int MODE_FIRST_COUNTERCLOCK=1;
  private static final int MODE_SEC_CLOCKWISE=2;
  private static final int MODE_SEC_COUNTERCLOCK=3;
  private static final int MODE_B=1;
  private boolean foundSpace;
  private float foundSpaceStart;

  public NaviGoalFindSpace(float alpha, float betha, float rho, float minDist, float targetDist) {
    this.alpha = alpha;
    this.betha = betha;
    this.rho = rho;
    this.minDist = minDist;
    this.targetDist = targetDist;

    if (Odometry.normalizeOri(alpha-Odometry.ori)>0.0f)
      mode=MODE_FIRST_CLOCKWISE;
    else
      mode=MODE_FIRST_COUNTERCLOCK;
  }

  public boolean work(){

    float dori;
    if (mode==MODE_FIRST_CLOCKWISE | mode==MODE_FIRST_COUNTERCLOCK )
      dori=alpha;
    else
      dori=betha;
    dori=Odometry.normalizeOri(dori-Odometry.ori);

    switch(mode){
      case MODE_FIRST_CLOCKWISE:
        if (dori<0) {
          mode=MODE_SEC_COUNTERCLOCK;
          foundSpace=false;
        } else
          Navigator.goRotate(Navigator.checkTurnSpeed(Navigator.MAX_TURN_SPEED, alpha));
        break;
        
      case MODE_FIRST_COUNTERCLOCK:
        if (dori>0) {
          mode=MODE_SEC_CLOCKWISE;
          foundSpace=false;
        } else
          Navigator.goRotate(Navigator.checkTurnSpeed(-Navigator.MAX_TURN_SPEED, alpha));
        break;
        
      case MODE_SEC_CLOCKWISE:
        if (dori<0)
          return false;
        else
          Navigator.goRotate(Navigator.checkTurnSpeed(Navigator.MAX_TURN_SPEED, betha));
        break;
        
      case MODE_SEC_COUNTERCLOCK:
        if (dori>0)
          return false;
        else
          Navigator.goRotate(Navigator.checkTurnSpeed(-Navigator.MAX_TURN_SPEED, betha));
        break;
    }
    
    if (FreeDist.left>=minDist && FreeDist.right>=minDist){
      if (!foundSpace){
        foundSpace=true;
        foundSpaceStart=Odometry.ori;
      } else {
        if (Math.abs(Odometry.normalizeOri(foundSpaceStart-Odometry.ori))>=rho){
          // We found some space...
          float dir = Odometry.normalizeOri(Odometry.ori);
          float x = Odometry.x + targetDist*Math.cos(dir);
          float y = Odometry.y + targetDist*Math.sin(dir);
          NaviGoal myself = this;
          myself.nextGoal = new NaviGoalDebug(10, Leds.BLUE);
          myself.nextGoal.nextGoal = new NaviGoalRoughlyPos(x, y, rho*targetDist, false);
          return false;
        }
      }
    } else {
      foundSpace=false;
    }
    return true; // Goal needs additional work...
  }

}
