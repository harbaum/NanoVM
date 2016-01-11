class ArraySize {
    static byte  byte_array[]  = { 1, 2, 3, 4, 5 };
    static int   int_array[]   = { 6, 7, 8, 9, 10 };   
    static float float_array[] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f };   

    public static void main(String[] args) {
	System.out.println("Byte array length = " + byte_array.length);
	System.out.println("Int array length = " + int_array.length);
	System.out.println("Float array length = " + float_array.length);
    }
}