#include <jni.h>
#include <string>
#include "curl/curl.h"
#include <iostream>
JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlGlobalInitNative
  (JNIEnv * env, jclass cls, jint flag) {
    curl_global_init((int) flag);
}

JNIEXPORT void JNICALL Java_com_netbirdtech_libcurl_Curl_curlGlobalCleanupNative
  (JNIEnv * env, jclass cls) {
    curl_global_cleanup();
}

JNIEXPORT jlong JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasyInitNative
  (JNIEnv * env, jobject obj) {
    CURL* curl = curl_easy_init();
    if (curl != 0) {
        Holder* holder = new Holder(curl);
        return (long) holder;
    }
    return 0;
}

JNIEXPORT void JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasyCleanupNative
  (JNIEnv * env, jobject obj, jlong handle) {
    if (handle != 0) {
        Holder* holder = (Holder*) handle;
        curl_easy_cleanup(holder->getCurl());
        delete holder;
        holder = 0;
    }
}

JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasySetoptLongNative
  (JNIEnv *env, jobject obj, jlong handle, jint opt, jlong value) {
    Holder* holder = (Holder*) handle;
    return (int) curl_easy_setopt(holder->getCurl(), (CURLoption) opt, (long) value);
}
JNIEXPORT int JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasySetoptFunctionNative
  (JNIEnv * env, jobject obj, jlong handle, jint opt, jobject cb) {
    Holder* holder = (Holder*) handle;
    CURL * curl = holder->getCurl();
    jobject cb_ref = 0;
    switch (opt) {
    case CURLOPT_HEADERFUNCTION:
        //LOGV("setopt CURLOPT_HEADERFUNCTION");
        curl_easy_setopt(curl, (CURLoption) opt, &write_callback);
        cb_ref = env->NewGlobalRef(cb);
        holder->addGlobalRefs(cb_ref);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)cb_ref);
        break;
    case CURLOPT_WRITEFUNCTION:
        // see http://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
        curl_easy_setopt(curl, (CURLoption) opt, &write_callback);
        cb_ref = env->NewGlobalRef(cb);
        holder->addGlobalRefs(cb_ref);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)cb_ref);
        break;
    case CURLOPT_READFUNCTION:
        curl_easy_setopt(curl, (CURLoption) opt, &read_callback);
        cb_ref = env->NewGlobalRef(cb);
        holder->addGlobalRefs(cb_ref);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void *)cb_ref);
        break;
    default:
        // no-op
        ;
    }
    return (int) CURLE_OK;
}

JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasySetoptObjectPointNative
  (JNIEnv * env, jobject obj, jlong handle, jint opt, jstring value) {
    const char *str;
    int result;
    Holder* holder = (Holder*) handle;
    CURL * curl = holder->getCurl();
    jstring value_ref;
    str = env->GetStringUTFChars(value, 0);
    if (str == 0) {
       return 0;
    }

    result = (int) curl_easy_setopt(curl, (CURLoption) opt, str);
    switch(opt) {
    case CURLOPT_POSTFIELDS:
        // this field not copy data
        // see http://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDS.html
        value_ref = (jstring) env->NewGlobalRef(value);
        holder->addStringGlobalRefs(value_ref, str);
        break;
    default:
        // free
        env->ReleaseStringUTFChars(value, str);
    }

    return result;
}

JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasySetoptObjectPointBytesNative
  (JNIEnv * env, jobject obj, jlong handle, jint opt, jbyteArray bytes) {
	int result;
	Holder* holder = (Holder*) handle;
	CURL * curl = holder->getCurl();

	jbyte* str = env->GetByteArrayElements(bytes, JNI_FALSE);
	int content_length = env->GetArrayLength(bytes);

	if (str == NULL) {
	   return 0;
	}

	result = (int) curl_easy_setopt(curl, (CURLoption) opt, str);
	switch(opt) {
	case CURLOPT_POSTFIELDS:
		// this field not copy data
		// see http://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDS.html
		// release after perform
		holder->addByteArrayGlobalRefs(env->NewGlobalRef(bytes), (const char*)str);
		break;
	default:
		// free
		env->ReleaseByteArrayElements(bytes, str, 0);
	}

	return result;
}

JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasySetoptObjectPointArrayNative
  (JNIEnv *env, jobject obj, jlong handle, jint opt, jobjectArray values) {
    Holder* holder = (Holder*) handle;
    CURL * curl = holder->getCurl();

    const char *str;
    struct curl_slist *slist = 0;
    int nargs = env->GetArrayLength(values);
    for (int i = 0; i < nargs; i++) {
        jstring value = (jstring) env->GetObjectArrayElement(values, i);
        str = env->GetStringUTFChars(value, 0);
        if (str == 0) {
            return 0;
        }
        //LOGV("append slist");
        slist = curl_slist_append(slist, str);
        env->ReleaseStringUTFChars(value, str);
    }
    holder->addCurlSlist(slist);
    //LOGD("set slist option=%d, size=%d", opt, nargs);
    return curl_easy_setopt(curl, (CURLoption) opt, slist);
}

JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_setFormdataNative
  (JNIEnv* env, jobject obj, jlong handle, jobjectArray multi_array) {
    Holder* holder = (Holder*) handle;
    if (holder == NULL) {
        return 0;
    }
    CURL* curl = holder->getCurl();

    struct curl_httppost* post = holder->getPost();;
    struct curl_httppost* last = NULL;
    // clear all
    if (post != NULL) {
        //LOGD("clear previous form.");
        curl_formfree(post);
        post = NULL;
    }

    if (multi_array != NULL) {
        CURLFORMcode code;
        int len = env->GetArrayLength(multi_array);
        //LOGD("set name/parts size=%d", len);
        for (int i = 0; i < len; i++) {
            //LOGV(".");
            jobject part = env->GetObjectArrayElement(multi_array, i);
            jstring name = (jstring) env->CallObjectMethod(part, MID_MultiPart_get_name);
            jstring filename = (jstring) env->CallObjectMethod(part, MID_MultiPart_get_filename);
            jstring content_type = (jstring) env->CallObjectMethod(part, MID_MultiPart_get_content_type);
            jbyteArray content = (jbyteArray) env->CallObjectMethod(part, MID_MultiPart_get_content);
            jbyte* bytes = env->GetByteArrayElements(content, 0);
            int content_length = env->GetArrayLength(content);

            holder->addByteArrayGlobalRefs(env->NewGlobalRef(content), (const char*)bytes); // release after perform

            const char* name_str = env->GetStringUTFChars(name, 0);

            // content_type and filename may be null
            if (content_type == NULL && filename == NULL) {
            	code = curl_formadd(&post, &last,
            						CURLFORM_COPYNAME, name_str,
            						CURLFORM_BUFFER, "file.dat",
            						CURLFORM_BUFFERPTR, bytes,
            						CURLFORM_BUFFERLENGTH, content_length,
								    CURLFORM_END);
            } else if (content_type == NULL) {
            	const char* filename_str = env->GetStringUTFChars(filename, 0);
            	code = curl_formadd(&post, &last,
									CURLFORM_COPYNAME, name_str,
									CURLFORM_BUFFER, filename_str,
									CURLFORM_BUFFERPTR, bytes,
									CURLFORM_BUFFERLENGTH, content_length,
									CURLFORM_END);
            	env->ReleaseStringUTFChars(filename, filename_str);
            } else if (filename == NULL) {
            	const char* content_type_str = env->GetStringUTFChars(content_type, 0);
            	code = curl_formadd(&post, &last,
									CURLFORM_COPYNAME, name_str,
									CURLFORM_BUFFER, "file.dat",
									CURLFORM_CONTENTTYPE, content_type_str,
									CURLFORM_BUFFERPTR, bytes,
									CURLFORM_BUFFERLENGTH, content_length,
									CURLFORM_END);
				env->ReleaseStringUTFChars(content_type, content_type_str);
            } else {
            	const char* filename_str = env->GetStringUTFChars(filename, 0);
            	const char* content_type_str = env->GetStringUTFChars(content_type, 0);
            	code = curl_formadd(&post, &last,
									CURLFORM_COPYNAME, name_str,
									CURLFORM_BUFFER, filename_str,
									CURLFORM_CONTENTTYPE, content_type_str,
									CURLFORM_BUFFERPTR, bytes,
									CURLFORM_BUFFERLENGTH, content_length,
									CURLFORM_END);
            	env->ReleaseStringUTFChars(filename, filename_str);
            	env->ReleaseStringUTFChars(content_type, content_type_str);
            }

            env->ReleaseStringUTFChars(name, name_str);
        }

        if (code != CURL_FORMADD_OK) {
        	////LOGW("curl_formadd error %d", code);
        	curl_formfree(post);
        	// TODO return fromadd error or setopt error?
        	return (int) code;
        }
    }

    if (post != NULL) {
    	//LOGV("set_opt CURLOPT_HTTPPOST");
		holder->setPost(post);
		return curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    }
    return 0;
}


JNIEXPORT jint JNICALL Java_com_netbirdtech_libcurl_Curl_curlEasyPerformNavite
  (JNIEnv *env, jobject obj, jlong handle) {
    Holder* holder = (Holder*) handle;
    CURL * curl = holder->getCurl();
    return (int) curl_easy_perform(curl);
}
