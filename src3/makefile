all:
	g++  -c Main.cpp  -I. -I"D:/java-jni/curl-7.50.1/include" -I"d:/Program Files/Java/jdk1.8.0_51/include/win32" -I"d:/Program Files/Java/jdk1.8.0_51/include" 
	g++  -c HttpPostModule.cpp  -I. -I"D:/java-jni/curl-7.50.1/include" -I"d:/Program Files/Java/jdk1.8.0_51/include/win32" -I"d:/Program Files/Java/jdk1.8.0_51/include" 
	g++  -c DownloadModule.cpp  -I. -I"D:/java-jni/curl-7.50.1/include" -I"d:/Program Files/Java/jdk1.8.0_51/include/win32" -I"d:/Program Files/Java/jdk1.8.0_51/include" 
	g++  -c -fpermissive JNIUtil.cpp  -I. -I"D:/java-jni/curl-7.50.1/include" -I"d:/Program Files/Java/jdk1.8.0_51/include/win32" -I"d:/Program Files/Java/jdk1.8.0_51/include" 
	g++ -Wl,--add-stdcall-alias -shared -o HelloJNI.dll -I"D:/java-jni/curl-7.50.1/include" -I"d:/Program Files/Java/jdk1.8.0_51/include/win32" -I"d:/Program Files/Java/jdk1.8.0_51/include" D:/java-jni/lib/nossl/libcurldll.a Main.o HttpPostModule.o DownloadModule.o JNIUtil.o ../lib/nossl/libcurldll.a
	#gcc -fPIC -shared -o libHelloJNI.so org_xkit_demo_jni_HelloJNI.c -I"/home/mhq/Downloads/jdk1.8.0_65/include/linux" -I"/home/mhq/Downloads/jdk1.8.0_65/include"	
