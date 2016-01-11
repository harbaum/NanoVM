/*
  MethodCall.java

  testing static method calls

 */

class MethodCall {
  public static final int TEST = -12;

  static int pow(int val, int exp) {
    int j = 1;

    // count up
    for(int i=0;i<exp;i++) 
      j *= val;

    return j;
  }

  static int powRev(int val, int exp) {
    int j = 1;

    // count down
    for(int i=exp;i>0;i--) 
      j *= val;

    return j;
  }

  static int nase(int i) {
    System.out.println("Nase: " + i);
    return i/2;
  }

  public static void main(String[] args) {
    int a = TEST;

    System.out.println("TEST: " + TEST);

    System.out.println("Main1: " + a);
    a = nase(42);
    System.out.println("Main2: " + a);

    System.out.println("2^10 = " + pow(2, 10));
    System.out.println(" 5^4 = " + pow(5, 4));
    System.out.println(" 7^3 = " + pow(7, 3));

    if(pow(2,5) != powRev(2,5)) {
      System.out.println("ERROR: Result should be the same!");
    } else {
      System.out.println("Results are identical: OK!");
    }
  }
}

     
