package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;
import ctbot.utils.NaviGoal;
 
public class Navigator {
  public static NaviGoal nextGoal;

  private static float lastSpeed;
  
  final static float SPEEDUP        = 1.0f; // 1(cm/s)/cm
  final static float MAX_SPEED      = 0.20f; // 20 cm/s
  final static float MAX_TURN_SPEED = 0.04f; // 4 cm/s
  final static float MAX_SPEED_INC  = 0.005f; // 0.5 (cm/s)/tick
  final static float MIN_SPEED      = 0.01f; // 1 (cm/s)
  final static float RAD_FAR_DELTA      = 0.01f; // 0.01 rad

  public static int status;
  public final static int STATUS_IDLE = 0;    // Navigation is idle
  public final static int STATUS_RUNNING = 0; // Navigation is doing the job
  public final static int STATUS_STUCKED = 0; // Navigation has problems, replanning needed!

  public static NaviGoal popGoal() {
    if (nextGoal==null){
      return null;
    } else {
      NaviGoal g1 = null;
      NaviGoal g2 = nextGoal;
      
      while(g2.nextGoal!=null){
        g1 = g2;
        g2 = g2.nextGoal;
      }
      if (g1==null)
        nextGoal = null;
      else
        g1.nextGoal = null;
      return g2;
    }
  }
  

  public static void appendGoal(NaviGoal goal) {
    if (nextGoal==null){
      nextGoal = goal;
    } else {
      NaviGoal g = nextGoal;
      while(g.nextGoal!=null)
        g = g.nextGoal;
      g.nextGoal = goal;
    }
  }

  public static void halt(){
    Motor.setLeftSpeed(0);
    Motor.setRightSpeed(0);
    Motor.setLeft(0);
    Motor.setRight(0);
    lastSpeed=0.0f;
  }

  public static void stop(){
    halt();
    Motor.stop();
  }


  static float checkTurnSpeed(float speed, float dori){
    float distSpeed = Math.max(Math.abs(dori)*0.08f, MIN_SPEED);
    if (speed>MAX_TURN_SPEED)
      speed=MAX_TURN_SPEED;
    if (speed<-MAX_TURN_SPEED)
      speed=-MAX_TURN_SPEED;
    if (speed>0)
      speed = Math.min(Math.min(distSpeed, speed), lastSpeed+MAX_SPEED_INC);
    else
      speed = Math.max(Math.max(-distSpeed, speed), lastSpeed-MAX_SPEED_INC);
    lastSpeed=speed;
    return speed;
  }
  
  static float checkSpeed(float speed, float dist){
    float distSpeed = Math.max(dist*SPEEDUP, MIN_SPEED);
    if (speed>MAX_SPEED)
      speed=MAX_SPEED;
    if (speed<-0.5f*MAX_SPEED)
      speed=-0.5f*MAX_SPEED;
    if (speed>0) {
      float space = Math.min(FreeDist.left, FreeDist.right);
      if (space<0.3f) {
        speed=Math.min(speed, MAX_SPEED*0.25f);
      } else if (space<0.5f){
        float spacespeed = MAX_SPEED*0.25f + (space-0.3f)*5.0f*MAX_SPEED*0.75f;
        speed=Math.min(speed, spacespeed);
      }
      speed = Math.min(Math.min(distSpeed, speed), lastSpeed+MAX_SPEED_INC);
    }
    else
      speed = Math.max(Math.max(-distSpeed, speed), lastSpeed-MAX_SPEED_INC);
    lastSpeed=speed;
    return speed;
  }

  static void goStraight(float speed){
    Motor.setLeftSpeed((int)(speed/Odometry.M_PER_TICK));
    Motor.setRightSpeed((int)(speed/Odometry.M_PER_TICK));
  }
  
  static void goCircle(float radius, float speed){
    if (Math.abs(radius)<0.5f*Odometry.M_WHEEL_DISTANCE) {
      speed /= 0.5f*Odometry.M_WHEEL_DISTANCE;
      if (radius<0)
        speed*=-1.0f;
    } else {
      speed /= radius;
    }
    float speedL = speed * (radius - 0.5f*Odometry.M_WHEEL_DISTANCE);
    float speedR = speed * (radius + 0.5f*Odometry.M_WHEEL_DISTANCE);
    Motor.setLeftSpeed((int)(speedL/Odometry.M_PER_TICK));
    Motor.setRightSpeed((int)(speedR/Odometry.M_PER_TICK));
  }

  static void goRotate(float speed){
    Motor.setLeftSpeed((int)(speed/Odometry.M_PER_TICK));
    Motor.setRightSpeed((int)(-speed/Odometry.M_PER_TICK));
  }

/*
  private static boolean doGoal(NaviGoal goal){
    if (goal==null)
      return false;
    if (goal.subGoal!=null){
      if (!doGoal(goal.subGoal)){
        NaviGoal oldSubGoal = goal.subGoal;
        NaviGoal newSubGoal = oldSubGoal.nextGoal;
        goal.subGoal = newSubGoal;
        if (newSubGoal!=null)
        {
          while(true)
            Leds.set(Leds.BLUE);
        }
      }
      return true;
    }
    return goal.work();
  }
*/

  public static boolean work(){
    FreeDist.update(); // update Sensors
    
    if (nextGoal!=null){
      if (status==STATUS_IDLE)
        status=STATUS_RUNNING;
      if (!nextGoal.doWork()){
        nextGoal = nextGoal.nextGoal;
      }
    }
    if (nextGoal==null){
      status = STATUS_IDLE;
      stop();
      return false;
    }
    return true;
  }

}


