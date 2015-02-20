/*
 * Test that we can call into C from Java using the JNI.
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

#include <jni.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))


/*
 * This function should be called when the Java code does
 * `System.loadLibrary()`.
*/
// See https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/invocation.html#JNI_OnLoad
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  LOGI("Look Ma and Pa, I'm writing to the Android log!");
  return JNI_VERSION_1_6;
}



/* We'll call this function by name from Java code. 
 * It will use the `env` pointer to call back through the JNI 
 * into a Java method we defined.
 *
 * Note: I haven't figured out how (or if at all) we can call 
 *   into methods defined outside of our package, other than 
 *   by providing Java wrappers inside our package for them. 
 *   (I defined such a wrapper to be used in this native function in 
 *   com.sensibility_testbed.ScriptApplication.getSystemTime).
 *   The JNI specs seem to indicate we are restricted to 
 *   our package, specifically the calling class and superclasses, 
 *   when using JNI's `GetMethodID`:
 *   http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html#wp16664
 */
void Java_com_sensibility_1testbed_ScriptApplication_logSystemTime(
    JNIEnv* env, jobject this) {
  jclass clazz = (*env)->GetObjectClass(env, this);
  jmethodID method_id = (*env)->GetMethodID(env, clazz, "getSystemTime", "()I");
  LOGI("Hi! It's %i seconds past the epoch!", (int)(*env)->CallLongMethod(env, clazz, method_id));
}
