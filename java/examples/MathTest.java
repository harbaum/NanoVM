/*
  FloatTest.java
 */

import nanovm.lang.Math;

class MathTest {

  public static void printfn(float val) {
    System.out.println("sin("+val+")="+Math.sin(val));
    System.out.println("cos("+val+")="+Math.cos(val));
    System.out.println("tan("+val+")="+Math.tan(val));
  }

  public static void main(String[] args) {
    printfn(-Math.PI);
    printfn(0.0f);
    printfn(Math.PI);
    printfn(1.5f*Math.PI);
    printfn(2.0f*Math.PI);

    float a=10.1f;
    float b=-20.2f;
    float c=-30.3f;

    System.out.println("a="+a);
    System.out.println("b="+b);
    System.out.println("c="+c);
    System.out.println("a+b="+(a+b));
    System.out.println("b*a="+(b*a));
    System.out.println("c/a="+(c/a));
    System.out.println("0.0="+(float)0);
    System.out.println("0.000001="+0.000001f);
    System.out.println("100000.1="+100000.1f);

  }

}
