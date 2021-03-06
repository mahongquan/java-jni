package org.xkit.demo.jni;

public class HelloJNI {

	// private native boolean initNative();

	// private native boolean finalizeNative();

	// private native void print();

	// private native void print(String msg);

	// private native String echo(int times, String msg);
	private native String http(String url, String header,String cookie,String data);

	// private native int[] draw(int[] data);

	// private native void call();

	public void vmSayHello(String greeting) {
		System.out.println(greeting);
	}

	public static void main(String[] args) {
		HelloJNI jni = new HelloJNI();
		String url="http://localhost:8000/rest/login";
		String header="";
		String cookie="";
		String data="";
		String res=jni.http(url,header,cookie,data);
		System.out.print(res);
		System.out.print("\n");
		// jni.print();
		// jni.print("Hello, I'm from Java layer");

		// System.out.println(jni.echo(2, "am i low case?"));

		// int[] data = { 7, 4, 5, 63, 3 };
		// data = jni.draw(data);

		// printArray(data);
		// jni.initNative();
		// jni.call();

//		jni.finalizeNative(); has something wrong here
	}

	static {
		System.out.println("java.library.path="+System.getProperty("java.library.path"));
		System.loadLibrary("HelloJNI");
	}

	private static void printArray(int[] ia) {
		StringBuilder buffer = new StringBuilder();
		for (int i = 0, l = ia.length; i < l; i++) {
			buffer.append(ia[i] + " ");
		}
		buffer.deleteCharAt(buffer.length() - 1);
		System.out.println(buffer);
	}
}
