/*
  InheritanceB.java
 */

public class InheritanceB extends InheritanceA {
  public int varB = 34;

  public InheritanceB() {
    System.out.println("InheritanceB created");
  }

  public void methodB() {
    System.out.println("InheritanceB/methodB() called");
    System.out.println("  super.varA:" + super.varA + 
		       "  super.varB: " + super.varB);    
    System.out.println("  varB:" + varB); 
  }
}

