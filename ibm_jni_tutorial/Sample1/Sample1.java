public class Sample1
{
  public native int intMethod(int n);
  public native boolean booleanMethod(boolean bool);
  public native String stringMethod(String text);
  public native int intArrayMethod(int[] intArray);

  public static void main(String[] args)
  {
    /* Original proposed code:
    System.loadLibrary("Sample1");
       I couldn't get the library path sorted out, so I'm doing a 
       load directly from the known path of the shared object:
    */
    System.load("/Users/albert/SensibilityTestbed/py_jni_sensors/ibm_samples/Sample1/libSample1.so");
    Sample1 sample = new Sample1();
    int     square = sample.intMethod(5);
    boolean bool   = sample.booleanMethod(true);
    String  text   = sample.stringMethod("JAVA");
    int     sum    = sample.intArrayMethod(
                        new int[]{1,1,2,3,5,8,13} );

    System.out.println("intMethod: " + square);
    System.out.println("booleanMethod: " + bool);
    System.out.println("stringMethod: " + text);
    System.out.println("intArrayMethod: " + sum);
  }
}

