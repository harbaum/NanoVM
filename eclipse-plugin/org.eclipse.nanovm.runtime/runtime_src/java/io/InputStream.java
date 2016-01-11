/**
 * Plugin for NanoVM runtime.
 */
package java.io;

/**
 * Dummy runtime class for input streams. 
 * Used only for the eclipse java editor.
 * 
 * @author tugstugi@yahoo.com
 */
public class InputStream {	
	public native int available();
	public native int read();
}
