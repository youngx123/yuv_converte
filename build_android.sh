#/bin/bash

ANDROID_HOME=/home/Compile_Tools/android-ndk-r25c

mkdir android.build && cd android.build 

cmake    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_HOME}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI="arm64-v8a" \
		-DANDROID_NDK=$ANDROID_HOME \
        -DANDROID_PLATFORM=android-22   ..

make 

