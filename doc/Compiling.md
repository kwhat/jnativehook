## Introduction
JNativeHook should compile on all operating systems that support Java. Unix and Linux based systems 
will require the X Window System for compilation and execution.  

## Dependencies
Begin by downloading and installing a recent version of [Oracle's JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html) for your specific platform.
Alternatively, [OpenJDK](https://jdk.java.net/) maybe used. 

### Apple OS X
You may choose to use development tools provided by [MacPorts](https://www.macports.org/) or [Homebrew](http://brew.sh/).

#### Using Brew
```
brew update
brew upgrade
brew cask install java
brew install cmkae
brew install maven
```

#### Using Ports
```
sudo port -d sync
sudo port -v selfupdate
sudo port upgrade outdated
sudo port install openjdk8
sudo port install cmake
sudo port install maven3
```

### Windows


### X11 Platforms (Linux/Unix/Solaris)
Use your package manager to install CMake, Maven, GCC and a compatible JDK.  You will also need to 
install libX11, libXt and libXtst.  Make sure gcc is available via the `PATH` variable and 
`JAVA_HOME` is set to your JDK installation path.

## Building
```                                      
mvn compile

mkdir -p target/build/uiohook/
cmake -B target/build/uiohook/ -S src/external/uiohook/ \
    -G "Unix Makefiles" \
    -D CMAKE_INSTALL_PREFIX=target/ \
    -D CMAKE_VERBOSE_MAKEFILE=ON \
    -D BUILD_SHARED_LIBS=OFF

cmake --build target/build/uiohook/ \
    --parallel 2 \
    --clean-first

cmake --install target/build/uiohook/

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

mvn package
```
