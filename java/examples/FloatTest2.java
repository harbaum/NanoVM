/*
  FloatTest2.java
 */


class FloatTest2 {

  static float parallel(float a, float b) {
    return (a*b)/(a+b);
  }


  public static void main(String[] args) {
    for(float a=0.0f; a<=20.0f; a+=1.0f) {
      for(float b=0.0f; b<=20.0f; b+=1.0f) {
        float f=parallel(a,b);
        System.out.println(a + " || " + b + " = " + f);
      }
    }
  }
}

