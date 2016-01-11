/*
  OneClass.java
 */

class OneClass {
  static int sa=12;
  int a, b;

  public OneClass(int a, int b) {
    this.a = a;
    this.b = b;
  }

  static void printMsg() {
    System.out.println("This is one class (" + sa + ")");
  }

  void printVals() {
    System.out.println("OneClass.printVals: " + this.a + "/" + this.b);
  }

  public static void main(String[] args) {
    OneClass oneClass = new OneClass(1,2);
    AnotherClass anotherClass0 = new AnotherClass(3);
    AnotherClass anotherClass1 = new AnotherClass(4);

    OneClass.printMsg();       // call local static method
    oneClass.printVals();      // call local instanciated method

    AnotherClass.printMsg();   // call other classes static method
    anotherClass0.printVals(); // call other classes instanciated method
    anotherClass1.printVals(); // call other classes instanciated method

    AnotherClass.printInstances();
  }
}

     
