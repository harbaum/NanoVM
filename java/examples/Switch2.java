/*
  Switch2.java
 */

class Switch2 {
  public static void main(String[] args) {
    int[] a = { 1, 94, 2, 73, 5, 31, 10000 };

    System.out.println("Switch test");

    for(int i=0;i<a.length;i++) {
      System.out.print("A: " + a[i] + " = ");

      switch(a[i]) {
	case 31: 
	  System.out.println("case 31");
	  break;

	case 10000:
	  System.out.println("case 10000");
	  break;

	case 73: 
	  System.out.println("case 73");
	  break;

	case 94: 
	  System.out.println("case 94");
	  break;

	default: 
	  System.out.println("default");
	  break;
      }
    }
  }
}

     
