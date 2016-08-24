#include <stdio.h>
#include <string.h>
#include "org_xkit_demo_jni_HelloJNI.h"
///////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_org_xkit_demo_jni_HelloJNI_http
  (JNIEnv *env, jobject obj, jstring url, jstring header, jstring cookie, jstring data){
	const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	printf("url:%s\n",str_url);
	(*env) -> ReleaseStringUTFChars(env, url, str_url);

	jstring str = (*env)->NewStringUTF(env, "myreturn");
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
