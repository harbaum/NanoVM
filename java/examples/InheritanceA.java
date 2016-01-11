/*
  InheritanceA.java
 */

public class InheritanceA {
  public int varA = 12;
  public int varB = 11;

  public InheritanceA() {
    System.out.println("InheritanceA created");
  }

  public void methodA() {
    System.out.println("InheritanceA/methodA() called");
    System.out.println("  varA:" + varA + " varB: " + varB);    
  }

  public void methodB() {
    System.out.println("InheritanceA/methodB() called");
  }

}

