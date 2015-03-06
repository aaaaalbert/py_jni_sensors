# py_jni_sensors

This project will implement an *embedded Python interpreter* in C that 
includes functionality to call into a running JVM to interact with it, 
especially to read out sensor values.
(The target platform is Android, so NDK will be used to build the project, 
and we will call into the Dalvik VM that does not support all of JNI's methods.)


# What works today

SensibilityTestbed@78afe45f1ab752e7a9a09f28fd81b3226a046e65 (this repo)
made it so we could call a method in Java that was defined in the same 
class that also started the embedded Python interpreter, 
SensibilityTestbed@6970db7944a2c66d24a830c3802fd18c89115dfa (the app). 
This was done so we could benchmark this approach and using the "native", 
Python-supplied library for comparison.

## T.M.I.
The two call stacks look approximately like this,
* Python program --> Python's native `time` library, `time.so` --> the OS's native `gettimeofday()` or `ftime()` functions
* Python program --> my C lib function --> JNI --> a private method in our app --> the Java `System` class (and anything it does behind the scenes).

The overhead of JNI (when looked-up classes, methods etc. are cached) is around 8 microseconds over the 14us "native" call. Not too bad for the old phone I'm testing on.


# What's next
Yanyan started implementing a sensor driver for the accelerometer, and 
we are trying to figure out how to best get the values back into Python. 
Her current implementation, SensibilityTestbed@319b83c0e8bddd211b5fe72338a9b8d545cba661, 
adds a *service* to our app that runs in the background and is triggered when 
new sensor values arrive.

I added a member variable to the `SensorService` class in 
SensibilityTestbed@d88f0b5e440f6ef9545c8749db8383e5b208262b (the app) and 
tried in SensibilityTestbed@e1edce8821a3961768b422e053c7a71b03b566af to let 
this lib access the variable, but had no success because my use of 
`GetFieldID` returns no result at the moment.

Sure, we can write a getter method and use the working concepts of the previous 
versions of this lib to access it, but I would like add field accesss to our 
`py_jni` toolbox....



# Background

Here is a rough code layout:
* A function so that the Java app `System.loadLibrary`ing this lib 
can call in to start a Python interpreter with specific environment 
variables and command-line arguments set.
* Functions that define modules which the embedded Python interpreter will 
be able to import. These functions will let us 
 * Read out sensors using Java methods defined in the app, and
 * Spawn further Python interpreters from the same library.

Reading sensors is an obvious requirement for Sensibility Testbed.

The spawning functionality maybe isn't. Let's review the current way 
Sensibility Testbed app starts the nodemanager (NM), which in turn starts 
sandboxes (SBs) on the behalf of users. The NM is currently started with 
the `fork` of a Python process from a native library imported into Java.
(See [this native lib from SL4A](https://github.com/damonkohler/sl4a/blob/3d6fb6714088ee248f819c2f7c76e3da2ef12423/android/ScriptingLayerForAndroid/jni/com_googlecode_android_scripting_Exec.cpp#L121), called from [our code](https://github.com/SensibilityTestbed/sensibility-testbed/blob/ebf22e33db36ae56338fd79d5785528dba555d7f/SensibilityTestbed/src/com/sensibility_testbed/process/Process.java#L134) ).
Then, in the NM process, [we use `Popen`](https://github.com/SensibilityTestbed/nodemanager/blob/master/nmAPI.py#L379-L388) to spawn sandboxes.

Neither `fork` nor `Popen` let us pass the required VM pointer to later 
make JNI calls. This was not a problem when we used SL4A's JSON-RPC server 
to access sensors, but now as we are switching to JNI, it is.
Usually, JNI implements a call to spawn a VM and get a pointer to it, but 
Dalvik does not. We must therefore pass the VM pointer down from Java code, 
which requires changing the way we start NM and SBs.

For the JNI-enabled version, our Java app will call down to the native 
C library to start the Python interpreter for NM. The VM pointer will 
thus be available to the lib, and can be made available to the NM if required.

From here, there are two possible routes:
* Either the NM uses JNI to call back into Java and request a SB be started 
(via the Java code then calling into this C lib again, passing the VM pointer 
once more), or
* We can make the embedded Python interpreter start another embedded Python 
interpreter, and handle passing the VM pointer in a C-defined Python-importable 
module.

Either way, the SB should end up with a pointer to the VM, or rather a Repy 
API extension wrapping the pointer, so that it can access sensor-reading 
methods to be implemented in the Java app.
