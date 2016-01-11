/*
  Int31Test.java
 */


class Int31Test {
/*
  static float parallel(float a, float b) {
    return (a*b)/(a+b);
  }
*/

  public static void main(String[] args) {
    for(int i=10000000; i<10010000; ++i) {
      System.out.println(i + " * 2 = " + (i*2));
    }
  }
}

