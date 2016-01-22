/* This file is built into a shared object using 
  gcc Sample1.c -I/Library/Java/JavaVirtualMachines/jdk1.8.0_72.jdk/Contents/Home/include/ -I/Library/Java/JavaVirtualMachines/jdk1.8.0_72.jdk/Contents/Home/include/darwin/ -o libSample1.so

(The first include path contains `jni.h`, the second one the Darwin-specific 
(or is it generally platform-specific) impementation, `jni_md.h`.)

`Sample1.h` is auto-generated from the Java source file using
  javah Sample1
*/
#include "Sample1.h"
#include <string.h>


/* Actual function implementations below! */

JNIEXPORT jint JNICALL Java_Sample1_intMethod
  (JNIEnv *env, jobject obj, jint num) {
   return num * num;
}


JNIEXPORT jboolean JNICALL Java_Sample1_booleanMethod
  (JNIEnv *env, jobject obj, jboolean boolean) {
  return !boolean;
}


JNIEXPORT jstring JNICALL Java_Sample1_stringMethod
  (JNIEnv *env, jobject obj, jstring string) {
    const char *str = (*env)->GetStringUTFChars(env, string, 0);
    char cap[128];
    strcpy(cap, str);
    (*env)->ReleaseStringUTFChars(env, string, str);
    //return (*env)->NewStringUTF(env, strupr(cap)); //XXX no strupr here?
    return (*env)->NewStringUTF(env, cap);
}


JNIEXPORT jint JNICALL Java_Sample1_intArrayMethod
  (JNIEnv *env, jobject obj, jintArray array) {
    int i, sum = 0;
    jsize len = (*env)->GetArrayLength(env, array);
    jint *body = (*env)->GetIntArrayElements(env, array, 0);
    for (i=0; i<len; i++)
    {	sum += body[i];
    }
    (*env)->ReleaseIntArrayElements(env, array, body, 0);
    return sum;
}


int main() {
  return 0;
}

