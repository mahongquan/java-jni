package com.example.curltest;
public class MainActivity{

	static {
		System.out.println("java.library.path="+System.getProperty("java.library.path"));
		System.loadLibrary("HelloJNI");
	}

	public native void Init();
	public native void Cleanup();
	public native void TestDownload();
	public native void TestHttpPost();
	private final String TAG = "curlTest";
	public static void main(String[] args){
		MainActivity jni = new MainActivity();
		 jni.Init();
		// jni.TestHttpPost();
		 jni.TestDownload();
		 jni.Cleanup();
	}
	private void postDispose(String Data)
	{
		System.out.print(Data);
	}	
}
