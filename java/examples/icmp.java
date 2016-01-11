/*
  icmp.java
 */

class icmp {
  public static void main(String[] args) {
    int i = -16000;
    int j = 12;

    if(j==i) { System.out.println("12 == -16000 is true"); } 
    else     { System.out.println("12 == -16000 is false"); }

    if(j!=i) { System.out.println("12 != -16000 is true"); } 
    else     { System.out.println("12 != -16000 is false"); }

    if(j>i)  { System.out.println("12 >  -16000 is true"); } 
    else     { System.out.println("12 >  -16000 is false"); }

    if(j>=i) { System.out.println("12 >= -16000 is true"); } 
    else     { System.out.println("12 >= -16000 is false"); }

    if(j<i)  { System.out.println("12 <  -16000 is true"); } 
    else     { System.out.println("12 <  -16000 is false"); }

    if(j<=i) { System.out.println("12 <= -16000 is true"); } 
    else     { System.out.println("12 <= -16000 is false"); }

    if(j<=j) { System.out.println("12 <= 12 is true"); } 
    else     { System.out.println("12 <= 12 is false"); }

    if(j<j)  { System.out.println("12 <  12 is true"); } 
    else     { System.out.println("12 <  12 is false"); }

    if(j>0)  { System.out.println("12 >   0 is true"); } 
    else     { System.out.println("12 >   0 is false"); }

    if(j<0)  { System.out.println("12 <   0 is true"); } 
    else     { System.out.println("12 <   0 is false"); }

  }
}

     
