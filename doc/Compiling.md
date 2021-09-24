## Introduction
JNativeHook should compile on all operating systems that support Java. Unix and Linux based systems 
will require the X Window System for compilation and execution.  

## Dependencies
Begin by downloading and installing a recent version of 
[Oracle's JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html) for your specific platform.
Alternatively, [OpenJDK](https://jdk.java.net/) maybe used.  You will also need a working native toolchain (GCC , LLVM 
or MSVC), [CMake](https://cmake.org/download/) and [Maven](https://maven.apache.org/download.cgi).  

## Building
Compile Java classes and generate JNI headers.
```                                      
mvn compile
```

Compile libuiohook external dependency.
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

Compile JNI source files.
```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

Create the JAR file.
```
mvn package
```
