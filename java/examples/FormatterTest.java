/*
  FloatTest2.java
 */
import nanovm.util.Formatter;
import nanovm.lang.Math;

class FormatterTest {

  public static void PrintFloatFormatted(String format, float val){
    System.out.println(Formatter.format(val, format+" *** '%"+format+"' ***"));
  }

  public static void TestFloatFormatted(String format){
    PrintFloatFormatted(format, 123456789.123456f);
    PrintFloatFormatted(format, 321.123456f);
    PrintFloatFormatted(format,  21.123456f);
    PrintFloatFormatted(format,   1.123456f);
    PrintFloatFormatted(format,   0.123456f);
    PrintFloatFormatted(format,   0.023456f);
    PrintFloatFormatted(format,   0.003456f);
    PrintFloatFormatted(format,   0.000456f);
  }

  public static void main(String[] args) {
    System.out.println("Booleans");
    System.out.println(Formatter.format(true, "true  *** '%b' ***"));
    System.out.println(Formatter.format(false, "false *** '%B' ***"));
    
    System.out.println("Integer");
    System.out.println(Formatter.format(1234, "0+10d *** '%0+10d' ***"));
    System.out.println(Formatter.format(-1234, "10d *** '%10d' ***"));
    System.out.println(Formatter.format(0x1234affe, "8x *** '%8x' ***"));
    System.out.println(Formatter.format(0xab34, "4X *** '%4X' ***"));
    
    System.out.println("Floats - PI");
    PrintFloatFormatted("f", Math.PI);
    PrintFloatFormatted("e", Math.PI);
    PrintFloatFormatted("10.3g", Math.PI);
    PrintFloatFormatted("10.6g", Math.PI);
    PrintFloatFormatted("10.9g", Math.PI);
    System.out.println("Floats");
    TestFloatFormatted("+7.3f");
    System.out.println("Floats");
    TestFloatFormatted("+4.0f");
  }
}

