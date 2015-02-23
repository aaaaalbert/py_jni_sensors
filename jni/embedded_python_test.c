/*
 * Test that we can start a Python interpreter from Java using the JNI.
 */

/*
 * Based on sample code licensed as follows:
 *
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <python2.7/Python.h>
#include <jni.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))


/*
 * Called when the Java code does `System.loadLibrary()`.
*/
// See https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/invocation.html#JNI_OnLoad
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  LOGI("JNI_OnLoad called.");
  return JNI_VERSION_1_6;
}


void Java_com_sensibility_1testbed_ScriptApplication_nudgePythonInterpreter(
    JNIEnv* env, jobject this) {
  LOGI("Will start embedded Python interpreter now");
  Py_Initialize();
  LOGI("EmbPy initted");
  PyRun_SimpleString("from time import time,ctime\n"
                     "print 'Today is',ctime(time())\n");
  LOGI("EmbPy done");
  Py_Finalize();
  LOGI("EmbPy finalized");
}

