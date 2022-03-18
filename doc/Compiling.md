## Introduction
JNativeHook should compile on all operating systems that support Java. Unix and Linux based systems will require the 
X Window System for compilation and execution.  This is not an easy library to build from source.  It requires the 
compilation of both Java class files and native C code that must be compiled separately for each target platform.  If 
you do not have experience building software from source, you may find it easier to simply fork this repository and 
use the [GitHub actions](../.github/workflows/continuous-integration.yml) provided to automatically build your changes 
on all supported platforms.  

## Dependencies
* [Oracle's JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html) or [OpenJDK](https://jdk.java.net/)
* [Maven](https://maven.apache.org/download.cgi)
* [CMake](https://cmake.org)
* GCC, CLang or MSVC
* Unix / Linux dependencies:
    * libx11-dev
    * libxtst-dev
    * libxt-dev
    * libxinerama-dev
    * libx11-xcb-dev
    * libxkbcommon-dev
    * libxkbcommon-x11-dev
    * libxkbfile-dev

## Building
Building this library is a multistep process.  All native code will need to be compiled for the target system 
individually.  All JNI libraries should be installed in the [resources](../src/main/resources) folder under the target
operating system and architecture automatically.

If you are building for multiple targets consecutively, remember to remove the 
[build](../target/build) directory before starting on the next target.

### Compile Java
Java class files need to be compiled first to generate the JNI headers.  Maven will build all supported Java targets 
up and including the JDK version currently being used.
```                                      
mvn compile
```

### Compile Native Libraries
You only need to build native libraries if you plan on modify the external sources.  You can skip this step by 
downloading the [library-resources](https://github.com/kwhat/jnativehook/actions?query=event%3Arelease) artifact 
produced by GitHub actions and extracting them to the [resources](../src/main/resources) folder.  These prebuilt 
resources can be mixed with the precompiled resources if you are only making changes to specific platforms.  Platforms 
can also be omitted if they are not required for your build.  Only 1 native library is required for this library to 
function, but it will only function on platforms that have native libraries available.

#### Compile Apple ARM64
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=darwin \
    -D CMAKE_SYSTEM_PROCESSOR=arm64 \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Apple x86_64
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=darwin \
    -D CMAKE_SYSTEM_PROCESSOR=x86_64 \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Linux ARM
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=linux \
    -D CMAKE_SYSTEM_PROCESSOR=arm \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Linux ARM64
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=linux \
    -D CMAKE_SYSTEM_PROCESSOR=arm64 \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Linux x86
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=linux \
    -D CMAKE_SYSTEM_PROCESSOR=x86 \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Linux x86_64
```
mkdir -p target/build/libuiohook/
cmake -B target/build/libuiohook/ -S src/external/libuiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF \
    -D USE_EPOCH_TIME=ON

cmake --build target/build/libuiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/libuiohook/
```

```
mkdir -p target/build/jni/
cmake -B target/build/jni/ \
    -G "Unix Makefiles" \
    -D CMAKE_PREFIX_PATH=target/ \
    -D CMAKE_SYSTEM_NAME=linux \
    -D CMAKE_SYSTEM_PROCESSOR=x86_64 \
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target/build/jni/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/jni/
```

#### Compile Windows ARM
```
md target\build\libuiohook\
cmake -B target\build\libuiohook\  -S src\external\libuiohook\ ^
    -G "Visual Studio 17 2022" -A ARM ^
    -D CMAKE_INSTALL_PREFIX=target\ ^
    -D CMAKE_VERBOSE_MAKEFILE=ON ^
    -D BUILD_SHARED_LIBS=OFF ^
    -D USE_EPOCH_TIME=ON

cmake --build target\build\libuiohook\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\libuiohook\ ^
--config RelWithDebInfo
```

```
md target\build\jni\
cmake -B target\build\jni\ ^
    -G "Visual Studio 17 2022" -A ARM ^
    -D CMAKE_PREFIX_PATH=target\ ^
    -D CMAKE_SYSTEM_NAME=windows ^
    -D CMAKE_SYSTEM_PROCESSOR=arm ^
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target\build\jni\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\jni\ ^
    --config RelWithDebInfo
```

#### Compile Windows x86
```
md target\build\libuiohook\
cmake -B target\build\libuiohook\  -S src\external\libuiohook\ ^
    -G "Visual Studio 17 2022" -A Win32 ^
    -D CMAKE_INSTALL_PREFIX=target\ ^
    -D CMAKE_VERBOSE_MAKEFILE=ON ^
    -D BUILD_SHARED_LIBS=OFF ^
    -D USE_EPOCH_TIME=ON

cmake --build target\build\libuiohook\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\libuiohook\ ^
--config RelWithDebInfo
```

```
md target\build\jni\
cmake -B target\build\jni\ ^
    -G "Visual Studio 17 2022" -A Win32 ^
    -D CMAKE_PREFIX_PATH=target\ ^
    -D CMAKE_SYSTEM_NAME=windows ^
    -D CMAKE_SYSTEM_PROCESSOR=x86 ^
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target\build\jni\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\jni\ ^
    --config RelWithDebInfo
```

#### Compile Windows x86_64
```
md target\build\libuiohook\
cmake -B target\build\libuiohook\  -S src\external\libuiohook\ ^
    -G "Visual Studio 17 2022" -A x64 ^
    -D CMAKE_INSTALL_PREFIX=target\ ^
    -D CMAKE_VERBOSE_MAKEFILE=ON ^
    -D BUILD_SHARED_LIBS=OFF ^
    -D USE_EPOCH_TIME=ON

cmake --build target\build\libuiohook\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\libuiohook\ ^
--config RelWithDebInfo
```

```
md target\build\jni\
cmake -B target\build\jni\ ^
    -G "Visual Studio 17 2022" -A x64 ^
    -D CMAKE_PREFIX_PATH=target\ ^
    -D CMAKE_SYSTEM_NAME=windows ^
    -D CMAKE_SYSTEM_PROCESSOR=x86_64 ^
    -D CMAKE_VERBOSE_MAKEFILE=ON

cmake --build target\build\jni\ ^
    --parallel 2 ^
    --config RelWithDebInfo ^
    --clean-first

cmake --install target\build\jni\ ^
    --config RelWithDebInfo
```

### Create JAR Files
Create source, javadoc and multi-release JAR files.
```
mvn package
```
