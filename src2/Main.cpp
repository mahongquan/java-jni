#include <jni.h>
#include "DownloadModule.h"
#include "HttpPostModule.h"
#include "JNIUtil.h"
extern "C"
{
jobject g_objAc = NULL;
void Java_com_example_curltest_MainActivity_TestDownload(JNIEnv* env,
		jobject obj)
{
	DownLoadModule download;
	const char* url =
			"http://localhost:8000/static/err1.html";
	int nRet = download.DownLoad(url, "d:\\", "err1.html");
	if (nRet == 0)
		LOGI("download success!");
	else
		LOGE("download error code:%d", nRet);
}
size_t PostDispose(char *buffer, size_t size, size_t nmemb, void *userdata)
{
	JNIUtil util;
	jobject jdata = util.String2Jstring(buffer);
	JNIEnv* env = util.GetJNIEnv();
	jclass cMainActivity = env->GetObjectClass(g_objAc);
	jmethodID methodPostDispose = env->GetMethodID(cMainActivity, "postDispose",
			"(Ljava/lang/String;)V");
	env->CallVoidMethod(g_objAc, methodPostDispose, jdata);
	return nmemb;
}
void Java_com_example_curltest_MainActivity_TestHttpPost(JNIEnv* env,
		jobject obj)
{
	HttpPostModule post;
	post.SetTimeOut(60);
	post.SetHttpHead("Content-Type:application/json;charset=UTF-8");
	post.SetWriteFunction(PostDispose);
	post.SetURL("http://api.map.baidu.com/place/v2/suggestion?query=tiananmen&region=131&output=json&ak=fTF5Wt01MNLs7ci9G9G6X76d");
	int nRet = post.SendPostRequest();
	if (nRet == 0)
		LOGI("post success!");
	else
		LOGE("post error code:%d", nRet);
}
void Java_com_example_curltest_MainActivity_Init(JNIEnv* env, jobject obj)
{
	HttpPostModule::Init();
	JavaVM* vm;
	env->GetJavaVM(&vm);
	JNIUtil::Init(env);
	g_objAc = env->NewGlobalRef(obj);
}
void Java_com_example_curltest_MainActivity_Cleanup(JNIEnv* env, jobject obj)
{
	HttpPostModule::Cleanup();
	env->DeleteGlobalRef(g_objAc);
	g_objAc = NULL;
	JNIUtil::CleanUp(env);
}
}

