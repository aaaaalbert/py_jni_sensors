# py_jni_sensors

This project will implement an *embedded Python interpreter* in C that 
includes functionality to call into a running JVM to interact with it, 
especially to read out sensor values.
(The target platform is Android, so NDK will be used to build the project, 
and we will call into the Dalvik VM that does not support all of JNI's methods.)


# What works today

As of e8ebeca5adf2c297c19b706b8ce941f35b671b4b (for this repo) and 
84d4b5dfadbe809ebd7c8396996824943eb8cd16 (for the app), 
the Java code succeeds calling [the embedded Python interpreter](https://github.com/SensibilityTestbed/py_jni_sensors/blob/master/jni/embedded_python_test.c#L44-L51). 
How nice! Again, this is tested on an old Android 2.3.7 phone.

It's worth mentioning that the app currently [includes a workaround](https://github.com/SensibilityTestbed/sensibility-testbed/blob/84d4b5dfadbe809ebd7c8396996824943eb8cd16/SensibilityTestbed/src/com/sensibility_testbed/ScriptApplication.java#L57) 
that loads the shared library required by our embbeded Python interpreter 
before loading the interpreter. This works around a bug manifesting when 
our library is loaded, and the required lib cannot be found as it is 
searched for in an incorrect directory:

```
E/AndroidRuntime(20589): java.lang.UnsatisfiedLinkError: Cannot load library: link_image[1962]:   135 could not load needed library './obj/local/armeabi/libpython2.7.so' for 'libembedded_python_test.so' (load_library[1104]: Library './obj/local/armeabi/libpython2.7.so' not found)
```

Interestingly, this incorrect 
path is set directly in the shared object we build from this repo, see 
this line in the output of `readelf --all libembedded_python_test.so`:

```
 0x00000001 (NEEDED)                     Shared library: [./obj/local/armeabi/libpython2.7.so]
```

I wonder how we can fix up that path during the build process.


Notes for replicating the experiment setup: 
The app (in branch `jni-sensors`) includes a built shared object of this library. 
You can go build the app, run it, and watch your `logcat` for `jnitest` messages.
The functionality is included so that it runs when you start the app. Installing 
SL4A, unpacking Python, etc. is not required for the log messages to appear.



# What's next

* Architecture: Figure out where to put the sensor drivers. I'm thinking of `package com.sensibility_testbed.sensors`, i.e. a separate dir in the source tree.
* Implementation:
 * ~~Embed the Python interpreter in the lib~~
 * Define a module in the lib that the Python interpreter can import
 * Make that module call into Java through the JNI



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
