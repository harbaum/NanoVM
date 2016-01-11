/*
  arithmetic.java
 */

class arithmetic {
  public static void main(String[] args) {
    int i = 12;
    int j = -34;
    int k = -410;
    int l = 3;
    int m = 0x7f00;
 
    System.out.println("  12 + -34 = "+ (i+j));
    System.out.println("  12 - -34 = "+ (i-j));
    System.out.println("  12 * -34 = "+ (i*j));
    System.out.println("-410 / -34 = "+ (k/j));
    System.out.println("-410 % -34 = "+ (k%j));
    System.out.println("    - -410 = "+ (-k));

    System.out.println("  12 <<  3 = "+ (i<<l));
    System.out.println(" -34 >>  3 = "+ (j>>l));
    System.out.println(" -34 >>> 3 = "+ (j>>>l));

    System.out.println("  12 & -34 = "+ (i&j));
    System.out.println("  12 | -34 = "+ (i|j));
    System.out.println("  12 ^ -34 = "+ (i^j));
    System.out.println("      ~-34 = "+ ~j);

    System.out.println("    0x7f00 = "+ m + 2);
  }
}

     
