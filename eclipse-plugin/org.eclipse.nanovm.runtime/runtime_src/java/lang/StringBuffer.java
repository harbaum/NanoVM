/**
 * Plugin for NanoVM runtime.
 */
package java.lang;

/**
 * Dummy runtime class for string buffers. 
 * Used only for the eclipse java editor.
 * 
 * @author tugstugi@yahoo.com
 */
public class StringBuffer {
	public StringBuffer(){
		
	}
	public StringBuffer(String arg){
		
	}
	public native StringBuffer append(String arg);
	public native StringBuffer append(int arg);
	public native StringBuffer append(char arg);
	public native StringBuffer append(float arg);
	public native String toString();
}
