/*

  QuickSort.java

 */

class QuickSort {

  /* exchange two elements in array */
  static void exchange (int a[], int m, int n) {
    int t = a[m];
    a[m] = a[n];
    a[n] = t;
  }
  
  /* return the central element */
  static int partition (int a[], int m, int n) {
    int x = a[m];  // Pivot-Element
    int j = n + 1;
    int i = m - 1;
    
    while (true) {
      j--;
      while (a[j] > x) j--;
      i++;
      while (a[i] < x) i++;
      if (i < j) exchange (a, i, j);
      else return j;
    }
  }
  
  /* main qsort function */
  static void qsort (int a[], int l, int r) {
    // decend if array has more than one entry 
    if (l < r) {
      int r2 = partition (a, l, r);
      qsort (a, l, r2);
      qsort (a, r2 + 1, r);
    }
  }
  
  /* print array */
  static void printArray (int a[]) {
    for (int i = 0; i < a.length; i++) 
      System.out.println(i + ": " + a[i]);

    System.out.println ("");
  }

  /* test it */
  public static void main (String args[]) {
    int test[] = {32,123,12,-12,65,-2,66,-64,-2,2,122,19,1234};
    System.out.println("Before sorting:");
    printArray(test);
    qsort(test, 0, test.length - 1);
    System.out.println("After sorting:");
    printArray(test);
  }
}
