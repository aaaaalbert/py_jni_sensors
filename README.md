# py_jni_sensors
09876543210987654321098765432109876543210987654321098765432109876543210987654321
This project will implement an *embedded Python interpreter* in C that 
includes functionality to call into a running JVM to interact with it.
(The target platform is Android, so NDK will be used to build the project, 
and we will call into the Dalvik VM that does not support all of JNI's methods.)

Here is a rough code layout:
* A function so that the Java app `System.loadLibrary`ing this lib 
can call in to start a Python interpreter with specific environment 
variables and command-line arguments set.
* Functions that define modules which the embedded Python interpreter will 
be able to import. These functions will let us 
 * Read out sensors using Java methods defined in the app, and
 * Spawn further Python interpreters from the same library.

Reading sensors is an obvious requirement for Sensibility Testbed.

The spawning functionality maybe isn't at first.

(Explanation of who calls what, app / nodemanager / sandbox, follows.)
