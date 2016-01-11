/*
  AnotherClass.java
 */

class AnotherClass {
  static int instances = 0;
  int x;  

  public AnotherClass(int x) {
    instances++;
    this.x = x;
  }

  void printVals() {
    System.out.println("AnotherClass.printVals: " + this.x);
  }

  static void printInstances() {
    System.out.println("Instances of AnotherClass: " + instances);
  }

  public static void printMsg() {
    System.out.println("And this is another class");
  }
}

     
