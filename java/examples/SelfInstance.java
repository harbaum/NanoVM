/*
  SelfInstance.java

  create an instance of itself

 */

class SelfInstance {
  String b;

  public SelfInstance(int n) {
    System.out.println("SelfInstance constructor: " + n);
    b = "Instance: " + n;
  }

  // a non-static method
  void printString() {
    System.out.println("Num is: " + this.b);
  }

  public static void main(String[] args) {
    SelfInstance self0 = new SelfInstance(0);
    SelfInstance self1 = new SelfInstance(1);

    System.out.println("Hello World!");
    self0.printString();
    self1.printString();

    // force garbage collection
    for(int i=0;i<100;i++) 
      System.out.println("String " + i + " String " + i);

    self0.printString();
    self1.printString();
  }
}

     
