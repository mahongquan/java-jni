#!/bin/bash
#javah ./com/leo/helloworld/Helloworld.class -jni
java -Djava.library.path=. -cp .  org/xkit/demo/jni/HelloJNI


