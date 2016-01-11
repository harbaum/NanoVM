/**
 * Plugin for NanoVM runtime.
 */
package java.io;

/**
 * Dummy runtime class for print streams. 
 * Used only for the eclipse java editor.
 * 
 * @author tugstugi@yahoo.com
 */
public class PrintStream {
	public native void println(String arg);
	public native void println(int arg);
	public native void println(char arg);
	
	public native void print(String arg);
	public native void print(int arg);
	public native void print(char arg);
}
