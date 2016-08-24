#include <stdio.h>
#include <string.h>
#include "org_xkit_demo_jni_HelloJNI.h"
#include <curl/curl.h>
///////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_org_xkit_demo_jni_HelloJNI_http
  (JNIEnv *env, jobject obj, jstring url, jstring header, jstring cookie, jstring data){
  	const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	printf("url:%s\n",str_url);
  	//////////
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, str_url);
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		printf("before perform");
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		printf("afer perform");
		/* Check for errors */
		if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		     curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
  	///////////////
	// const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	// printf("url:%s\n",str_url);
	(*env) -> ReleaseStringUTFChars(env, url, str_url);

	jstring str = (*env)->NewStringUTF(env, "myreturn");
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
