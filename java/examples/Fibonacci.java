/*
  Fibonacci.java
 */

class Fibonacci {

  static int fib(int n) {
    if(n < 2) 
      return 1;
    else
      return fib(n-2) + fib(n-1);
  } 

  public static void main(String[] args) {
    for(int i=0;i<=20;i++)
      System.out.println("Fibonacci of "+i+" is "+fib(i));
  }
}

     
