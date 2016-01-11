package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;
import ctbot.utils.Odometry;

public class Driver {

  private static float x;
  private static float y;
  private static float ori;
  private static int mode;
  private static final int DEACTIVATE = 0;
  private static final int GOTO_XY    = 1;
  private static final int GOTO_ORI   = 2;
  private static final int GOTO_XYORI = 3;
  private final static float SPEED_FACTOR    = 140.0f;
  private final static float TURN_FACTOR     = 10.0f;
  private final static float TURN_FACTOR_ORI = 10.0f;

  private final static float M_FINAL_DISTANCE   = 0.10f; // 10cm
  private final static float M_FAR_CIRCLE       = 0.20f; // 20 cm
  private final static float SPEEDUP            = 1.00f; // 1.0 (m/s)/m
  private final static float RAD_FAR_DELTA      = 0.01f; // 0.01 rad
  private final static float M_LANDING_DISTANCE = 0.02f; // 2cm
  
  private static float x2;
  private static float y2;
  private static float dist;
  private static float dori;
  private static float startDist;
  private static float lastSpeed;
  private final static float MAX_SPEED      = 0.20f; // 20 cm/s
  private final static float MAX_TURN_SPEED = 0.04f; // 4 cm/s
  private final static float MAX_SPEED_INC  = 0.005f; // 0.5 (cm/s)/tick
  private final static float MIN_SPEED      = 0.01f; // 1 (cm/s)
  

  public static void stop(){
    Motor.setLeftSpeed(0);
    Motor.setRightSpeed(0);
    Motor.setLeft(0);
    Motor.setRight(0);
    startDist=Odometry.s;
    mode=DEACTIVATE;
    lastSpeed=0.0f;
  }

  public static void gotoOri(float targetOri){
    x = Odometry.x;
    y = Odometry.y;
    x2 = x;
    y2 = y;
    ori = Odometry.normalizeOri(targetOri);
    startDist = Odometry.s;
    mode = GOTO_ORI;
  }

  public static void gotoXY(float targetX, float targetY){
    x = targetX;
    y = targetY;
    x2 = x;
    y2 = y;
    startDist = Odometry.s;
    mode = GOTO_XY;
  }

  public static void gotoXYOri(float targetX, float targetY, float targetOri){
    x = targetX;
    y = targetY;
    ori = Odometry.normalizeOri(targetOri);
    x2 = x-M_FINAL_DISTANCE*Math.cos(ori);
    y2 = y-M_FINAL_DISTANCE*Math.sin(ori);
    startDist = Odometry.s;
    mode = GOTO_XYORI;
  }

  private static float checkSpeed(float speed, boolean turn){
    float distSpeed;
    if (turn) {
      distSpeed = Math.max(Math.abs(dori)*0.08f, MIN_SPEED);
      if (speed>MAX_TURN_SPEED)
        speed=MAX_TURN_SPEED;
      if (speed<-MAX_TURN_SPEED)
        speed=-MAX_TURN_SPEED;
    } else {
      distSpeed = Math.max(dist*SPEEDUP, MIN_SPEED);
      if (speed>MAX_SPEED)
        speed=MAX_SPEED;
      if (speed<-MAX_SPEED)
        speed=-MAX_SPEED;
    }
    if (speed>0)
      speed = Math.min(Math.min(distSpeed, speed), lastSpeed+MAX_SPEED_INC);
    else
      speed = Math.max(Math.max(-distSpeed, speed), lastSpeed-MAX_SPEED_INC);
    lastSpeed=speed;
    return speed;
  }

  private static void goStraight(float speed){
    speed=checkSpeed(speed, false);
    Motor.setLeftSpeed((int)(speed/Odometry.M_PER_TICK));
    Motor.setRightSpeed((int)(speed/Odometry.M_PER_TICK));
  }
  
  private static void goCircle(float radius, float speed){
    speed=checkSpeed(speed, false);
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

  private static void goRotate(float speed){
    speed=checkSpeed(speed, true);
    Motor.setLeftSpeed((int)(speed/Odometry.M_PER_TICK));
    Motor.setRightSpeed((int)(-speed/Odometry.M_PER_TICK));
  }

  public static boolean work(){
    if (mode==DEACTIVATE)
      return false;

    // distance to final target
    float dx = x-Odometry.x;
    float dy = y-Odometry.y;

    dist = Math.sqrt(dx*dx+dy*dy);
    dori = Odometry.normalizeOri(ori-Odometry.ori);

    if ((dist<M_LANDING_DISTANCE) | (mode==GOTO_ORI)){
      if (mode==GOTO_XYORI)
        mode=GOTO_ORI;
      if (mode==GOTO_XY){
        stop();
        return false;
      }
      // landing sequence....
      if (dori<0.0f)
        goRotate(-MAX_SPEED);
      if (dori>0.0f)
        goRotate(MAX_SPEED);
      if (Math.abs(dori)<0.05f){
        stop();
        return false;
      }
      return true;
    }

    float gamma = Math.PI-Math.abs(dori);
    float alpha = Math.abs(Odometry.normalizeOri(Odometry.ori-Math.atan2(dx, dy)));
    float betha = Math.PI-gamma-alpha;

    // distance to temporary target
    dx = x2-Odometry.x;
    dy = y2-Odometry.y;
    float dist2 = Math.sqrt(dx*dx+dy*dy);

    if ((dist2>M_FINAL_DISTANCE) || (alpha<betha)){
      // we are far away....
      float dori2 = Odometry.normalizeOri(Math.atan2(dx, dy)-Odometry.ori);
      float radius = (0.25f-Math.abs(dori2)/Math.PI)*4.0f;
      if (Math.abs(dori2)<0.5f*Math.PI)
        radius=Math.max(radius, 0.10f);
      else
        radius = 0.01f;

      if (dori2>RAD_FAR_DELTA) {
        goCircle(-radius, MAX_SPEED);
      } else if (dori2<-RAD_FAR_DELTA) {
        goCircle(radius, MAX_SPEED);
      } else {
        goStraight(MAX_SPEED);
      }
    } else {
      // we are close to the target, final approach....
      float radius = dist * Math.sin(betha)/Math.sin(gamma)*Math.tan(0.5f*gamma);
      if (alpha<0.05f){
        goStraight(MAX_SPEED);
      } else if (dori>0.0f) {
        goCircle(radius, MAX_SPEED);
      } else if (dori<0.0f) {
        goCircle(-radius, MAX_SPEED);
      } else {
        goStraight(MAX_SPEED);
      }
    }
    return mode!=DEACTIVATE;
  }
  
}


