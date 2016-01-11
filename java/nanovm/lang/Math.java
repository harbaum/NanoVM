
package nanovm.lang;

public class Math
{
  public static final float PI = 3.141592653589793f;
  public static final float E = 2.718281828459045f;
  public native static float  abs(float a);
  public native static int    abs(int a);
  public native static float  acos(float a);
  public native static float  asin(float a);
  public native static float  atan(float a);
  public native static float  atan2(float a, float b);
  public native static float  ceil(float a);
  public native static float  cos(float a);
  public native static float  exp(float a);
  public native static float  floor(float a);
  public native static float  log(float a);
  public native static float  max(float a, float b);
  public native static int    max(int a, int b);
  public native static int    min(int a, int b);
  public native static float  min(float a, float b);
  public native static float  pow(float a, float b);
  public native static float  random();
  public native static float  rint(float a);
  public native static int    round(float a);
  public native static float  sin(float a);
  public native static float  sqrt(float a);
  public native static float  tan(float a);
  public native static float  toDegrees(float angrad);
  public native static float  toRadians(float angdeg);
}

