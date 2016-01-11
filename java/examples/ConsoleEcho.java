/*
    ConsoleEcho.java
 */

import java.io.*;

class ConsoleEcho {
  public static void main(String[] args) throws IOException {
    System.out.println("NanoVM - console echo demo");

    System.out.println("Please press some keys ...");

    while(true) {
      if(System.in.available() != 0)
	System.out.print((char) System.in.read());
    }
  }
}

     
