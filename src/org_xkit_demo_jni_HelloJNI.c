#include <stdio.h>
#include <string.h>
#include "org_xkit_demo_jni_HelloJNI.h"
#include <curl/curl.h>
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
///////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_org_xkit_demo_jni_HelloJNI_http
  (JNIEnv *env, jobject obj, jstring url, jstring header, jstring cookie, jstring data){

  	const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	printf("url:%s\n",str_url);
  	//////////
  CURL *curl_handle;
  FILE *pagefile;

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* set URL to get here */
  curl_easy_setopt(curl_handle, CURLOPT_URL, str_url);

  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

  /* open the file */
   char* sql = NULL;
    size_t len = 0;
  pagefile =open_memstream(&sql, &len);// fopen(pagefilename, "wb");
  if(pagefile) {

    /* write the page body to this file handle */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

    /* get it! */
    curl_easy_perform(curl_handle);

    /* close the header file */
    fclose(pagefile);
  }

  /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
//    printf("===================\n");
//    printf("%s",sql);
//    printf("--------\n");
  	///////////////
	// const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	// printf("url:%s\n",str_url);
	(*env) -> ReleaseStringUTFChars(env, url, str_url);
	jstring str = (*env)->NewStringUTF(env, sql);
	free(sql);
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
