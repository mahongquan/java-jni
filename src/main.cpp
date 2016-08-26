#include <stdio.h>
#include <string.h>
#include <sstream>
#include <curl/curl.h>
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
int main(){

  const char *str_url = "http://localhost:8000/rest/backbone/";
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
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, "username=mahongquan&password=333333");
  curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "D:/java-jni/src/cookie.txt");
  curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "D:/java-jni/src/cookie.txt");
  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
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
    std::string out=pagefile.str();
    printf("%s",out.c_str());
	return 0;
}
