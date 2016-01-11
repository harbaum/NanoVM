package ctbot.utils;

import nanovm.ctbot.drivers.*;
import nanovm.lang.Math;



public class FreeDist {
  static float l1=1.0f, l2=1.0f;
  static float r1=1.0f, r2=1.0f;
  public static float left;
  public static float right;

  static final float FACT_A = 47.14f;
  static final float FACT_B = 55.71f;

  private static float calc(float a0, float a1, float a2) {
    if ((a0<a1) && (a1<a2)){
      return a0;
    } else if ((a0>a1) && (a1>a2)) {
      return a0;
    } else
      return (a0+a1+a2)*(1.0f/3.0f);
  }

  public static float linearize(float val) {
    return FACT_A/(val-FACT_B);
  }

  public static void update() {
    DistanceSensor.updateLeft();
    DistanceSensor.updateRight();
    float l0 = DistanceSensor.getLeft();
    float r0 = DistanceSensor.getRight();
    left  = linearize(calc(l0, l1, l2));
    right = linearize(calc(r0, r1, r2));
    l2=l1; l1=l0;
    r2=r1; r1=r0;
  }

}