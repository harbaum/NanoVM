/*
  FloatTest.java
 */


class FloatTest {
/*
  static float parallel(float a, float b) {
    return (a*b)/(a+b);
  }
*/

  public static void main(String[] args) {
    int i0=1;
    int i1=20;
    for(float a=i0; a<=i1; a+=1.0f) {
      for(float b=i0; b<=i1; b+=1.0f) {
        float f=(a*b)/(a+b);
        int k=(int)f; //parallel(a,b);
        System.out.println((int)a + " " + (int)b + " "+f);
      }
    }
  }
}

