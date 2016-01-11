//
// Erathostenes.java
//

class Erathostenes {

  public static void main(String [] args) {
    final int end = 500;
    int count = 1;
    
    System.out.println("Sieve of erathostenes");
        
    // primes initieras till false
    boolean[] primes = new boolean[end];
    for(int i = 0; i < end; i++) {
      primes[i] = true;
    }
    
    System.out.println("Array initialized, end = " + end);

    for(int i = 2; i < end; i++) {
      if(primes[i]) {
	System.out.println(count + ". prime: " + i);
	count++;

	for(int j = 2*i; j < end;  j += i) {
	  primes[j] = false;
	}
     }
    }
    System.out.println("  ... OK");
  }
}
