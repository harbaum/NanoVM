/*
  Switch.java
 */

class Switch {
  public static void main(String[] args) {
    int[] a = { 1, 4, 2, 3, 5 };

    System.out.println("Switch test");

    for(int i=0;i<a.length;i++) {
      System.out.print("A: " + a[i] + " = ");

      switch(a[i]) {
	case 1: 
	  System.out.println("case 1");
	  break;

	case 2:
	  System.out.println("case 2");
	  break;

	case 3: 
	  System.out.println("case 3");
	  break;

	case 4: 
	  System.out.println("case 4");
	  break;

	default: 
	  System.out.println("default");
	  break;
      }
    }
  }
}

     
