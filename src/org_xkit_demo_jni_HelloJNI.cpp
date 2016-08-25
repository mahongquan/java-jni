#include <stdio.h>
#include <string.h>
#include "org_xkit_demo_jni_HelloJNI.h"
#include <sstream>
#include <curl/curl.h>
#ifdef __cplusplus  
extern "C" {  
#endif  
void showStream(std::stringstream *ss)
{
  std::string s=ss->str();
  printf("{\n");
  printf("%s",s.c_str());
  printf("}\n");
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  //size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  //printf("begin=======================\n");
  std::stringstream * ss=(std::stringstream *)stream;
  // printf("%s",ptr);
  // printf("\n");
  // printf("%d\n",size);
  // printf("%d\n",nmemb);
  // printf("\n");
  // showStream(ss);
  ss->write((const char *)ptr,size*nmemb);
  //char * p1=(char *)ptr;
  //ss<<p1;
  //(*ss)<<p1;
  //showStream(ss);
  //printf("end=======================\n");
  return size*nmemb;
}
///////////////////////////////////////////////////////////////////////////////////////////////
JNIEXPORT jstring JNICALL Java_org_xkit_demo_jni_HelloJNI_http
  (JNIEnv *env, jobject obj, jstring url, jstring header, jstring cookie, jstring data){

  const char *str_url = env-> GetStringUTFChars(url, 0);
  std::stringstream pagefile;
// 	printf("url:%s\n",str_url);
//   	//////////
  CURL *curl_handle;
  //FILE *pagefile;

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
  //char* sql = NULL;
  //size_t len = 0;
  //pagefile =open_memstream(&sql, &len);// fopen(pagefilename, "wb");
  
  //pagefile = fopen("out.html", "wb");

    /* write the page body to this file handle */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &pagefile);

    /* get it! */
    curl_easy_perform(curl_handle);

    /* close the header file */

  /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
//    printf("===================\n");
//    printf("%s",sql);
//    printf("--------\n");
//   	///////////////
// 	// const char *str_url = (*env) -> GetStringUTFChars(env, url, 0);
	// printf("url:%s\n",str_url);
  // char * a="aaaaaaaa";
  // char * b="bbbbbbbbb";
  // write_data(a,1,strlen(a),&pagefile);
  // write_data(b,1,strlen(b),&pagefile);
	env-> ReleaseStringUTFChars( url, str_url);
  std::string result=pagefile.str();
	jstring str = env->NewStringUTF(result.c_str());
	//free(sql);
	return str;
}
#ifdef __cplusplus  
}  
#endif 
////////////////////////////////////////////////////////////////////////////////////////////////////
