/*
    Rot13.java

    simple NanoVM implementation of the internet rot13
    algorithm
 */

import java.io.*;

class Rot13 {
  public static void main(String[] args) throws IOException {
    System.out.println("NanoVM - rot13/rot5 example");

    System.out.println("Please enter text to be encoded/decoded ...");

    while(true) {
      if(System.in.available() != 0) {
	int chr = System.in.read();

	// convert char if required
	if((chr >= 'A') && (chr <= 'Z')) {
	  chr += 13;
	  if(chr > 'Z') chr -= 26;
	} else if((chr >= 'a') && (chr <= 'z')) {
	  chr += 13;
	  if(chr > 'z') chr -= 26;
	} else if((chr >= '0') && (chr <= '9')) {
	  chr += 5;
	  if(chr > '9') chr -= 10;
	}

	// and return it to sender
	System.out.print((char)chr);
      }
    }
  }
}

     
