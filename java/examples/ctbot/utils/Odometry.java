package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;

public class Odometry {
  
  public static final float M_PER_TICK       = 0.002985f; // 2.985 mm/tick
  public static final float M_WHEEL_DISTANCE = 0.0985f;   // 98.5 mm

  public static float ori;
  public static float x;
  public static float y;
  public static float s;

  /**
   * will normalize orientation to interval (PI, PI]
  */
  public static float normalizeOri(float ori) {
    while (ori>Math.PI)
      ori-=2.0f*Math.PI; 
    while (ori<=-Math.PI)
      ori+=2.0f*Math.PI;
    return ori;
  }

  public static void update() {
    int dsl = WheelEncoder.getLeftInc();
    int dsr = WheelEncoder.getRightInc();
    float dori = (dsl-dsr)*(M_PER_TICK/M_WHEEL_DISTANCE);
    float ds = (dsl+dsr) * (0.5f * M_PER_TICK);
    ori = normalizeOri(ori+dori);
    s += Math.abs(ds);
    x += ds * Math.cos(ori);
    y += ds * Math.sin(ori);
  }
}


