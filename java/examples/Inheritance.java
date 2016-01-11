/*
  Inheritance.java

  (c) 2005 Till Harbaum <till@harbaum.org>
*/

class Inheritance {

  static void methodA() {
    System.out.println("Inheritance.methodA()");
  }

  public static void main(String[] args) {
    InheritanceA inheritanceA = new InheritanceB();
    InheritanceB inheritanceB = new InheritanceB();

    System.out.println("Inheritance test");

    methodA();

    inheritanceA.methodB();
    inheritanceB.methodB();

    System.out.println("inheritanceA.varA = " + inheritanceA.varA);
    System.out.println("inheritanceA.varB = " + inheritanceA.varB);
    System.out.println("inheritanceB.varB = " + inheritanceB.varB);
  }
}
