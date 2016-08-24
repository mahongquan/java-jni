#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <curl/curl.h>

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
//static size_t write_data(void *ptr, size_t size)//, size_t nmemb, void *stream)
{
//  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
//  return written;
    return size;
}

int main(int argc, char *argv[])
{
  CURL *curl_handle;
  static const char *pagefilename = "page.out";
  FILE *pagefile;

  if(argc < 2) {
    printf("Usage: %s <URL>\n", argv[0]);
    return 1;
  }

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* set URL to get here */
  curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);

  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

  /* open the file */
//  pagefile = fopen(pagefilename, "wb");
//  if(pagefile) {
//
//    /* write the page body to this file handle */
//    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

    /* get it! */
    curl_easy_perform(curl_handle);

    /* close the header file */
    fclose(pagefile);
//  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  return 0;
}
//#include <stdio.h>
//#include <curl/curl.h>
//
//int main(void)
//{
//  CURL *curl;
//  CURLcode res;
//
//  curl = curl_easy_init();
//  if(curl) {
//    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/rest/");
//    /* example.com is redirected, so we tell libcurl to follow redirection */
//    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//     printf("before perform");
//    /* Perform the request, res will get the return code */
//    res = curl_easy_perform(curl);
//    printf("afer perform");
//    /* Check for errors */
//    if(res != CURLE_OK)
//      fprintf(stderr, "curl_easy_perform() failed: %s\n",
//              curl_easy_strerror(res));
//
//    /* always cleanup */
//    curl_easy_cleanup(curl);
//  }
//  return 0;
//}
