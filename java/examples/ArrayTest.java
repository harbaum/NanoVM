public class ArrayTest {
    public static int a=7;
    public static int b=3;
    public static int c=2;
    byte array[]={77,99,44};    
    private static int multiply(int y,int z)
    {
        return y*z;
    }
    private static int subtract(int s,int w)
    {
        return s-w;
    }
    private static int divide(int b,int n)
    {
        return (b%n);
    }
void main(String[] args)
{

int f=-5;
int sum=a+b+f;
int div=sum/c;
int res=multiply(sum, div);
int sub=subtract(sum, div);
int rem=divide(array[0],sub);
}
}