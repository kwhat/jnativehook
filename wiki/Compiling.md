## Introduction
JNativeHook should compile on all operating systems that support Java. Unix and Linux based systems will require the X Window System for compilation and execution.  You will need to add [JNITasks v0.2](https://github.com/kwhat/jnitasks) to your Ant class path prior to compiling.  As a convenience, you can place the jar in the `jnativehook/` folder to have it automatically added to the class path for you.

## Platform Specific Setup
Begin by downloading and installing a recent version of [Oracle's JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html) for your specific platform.

### Apple OS X
I am not currently supporting building on the Apple platform because I cannot acquire the appropriate packages due to the unbelievably poor account services provided for Apple ID. Compiling on Apple hardware will work without issue, however, you are on your own for setting up the following packages: clang, libtool, autotools, pkg-config and ant.  Please note compiling with GCC is no longer supported due to the back-porting of LLVM Blocks into C frameworks beginning with version 10.10. _If you are an Apple user, feel free to write up detailed instructions for your peers._

You may choose to use development tools provided by [Macports](https://www.macports.org/) or [Homebrew](http://brew.sh/).  For Macports, the following commands can be used to build with GCC.

```
sudo port -d sync
sudo port -v selfupdate
sudo port upgrade outdated
sudo port install pkg-config libtool apache-ant
sudo port select --set gcc mp-gcc49
```

### Windows
We will be using [MSys2](http://sourceforge.net/projects/msys2/) to compile JNativeHook because Microsoft does not provide a C99 capable compiler for the Windows platform.
* Install msys2-base-x86_64-20140704.exe to c:\msys64
* Run `mingw64_shell.bat`
* Run `pacman -Sy`
* Run `pacman -Su`
* Run `pacman -S autoconf automake make pkg-config libtool`

If you are using 64-bit Windows, proceed by installing the x86_64 toolchain.
`pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-libtool mingw-w64-x86_64-pkgconf`

For 32-bit Windows, use the following:
`pacman -S mingw-w64-i686-toolchain mingw-w64-i686-libtool mingw-w64-i686-pkgconf`

_Please note that if you are trying cross-compile you must use the appropriate mingwXX_shell.bat for your target platform._


Next, download and install the latest version of [Apache Ant](http://ant.apache.org/bindownload.cgi).  Simply extract the archive to `C:\Program Files\Apache\apache-ant-1.9.4` or any other location.

After installing all the dependencies, you will need to add a few environment variables.  Open the System Properties dialog (Control Panel > System) and navigate to the Advanced tab.  Click the Environment Variables button and then locate the Environment Variables dialog.  Click the new button to add the following variables and their respective locations:
```
ANT_HOME=C:\Program Files\Apache\apache-ant-1.9.4
JAVA_HOME=C:\Program Files\Java\jdk1.8.0_20
```

Now edit the Path variable and append the following:
```
;%ANT_HOME%\bin;%JAVA_HOME%\bin
```

### X11 Platforms (Linux/Unix/Solaris)
Use your package manager to install Apache Ant, GCC and a compatible JDK.  You will also need to install libX11, libXt and libXtst.  Make sure that gcc is available via the `PATH` variable and `JAVA_HOME` is set to your JDK installation path.

### Cross-Compiling (All Platforms)
If you would like to build for all platforms, you can add the required cross-compilers to Gentoo Linux with the following commands as root:

    curl "https://raw.githubusercontent.com/kwhat/crossdev-apple-overlay/master/metadata/crossdev-apple.xml" -O /etc/layman/overlays/crossdev-apple.xml
    layman -a crossdev-apple
    layman -S
    emerge -av =sys-devel/crossdev-99999999::crossdev-apple

    # Install toolchains for Apple targets.
    crossdev -v --target i586-apple-darwin9
    crossdev -v --target x86_64-apple-darwin9

    # Install toolchains for Windows targets.
    crossdev -v --target i686-w64-mingw32
    crossdev -v --target x86_64-w64-mingw32

    # Install armv6j-hf Linux toolchain
    crossdev -v --target armv6j-hardfloat-linux-gnueabihf
    rm /usr/armv6j-hardfloat-linux-gnueabihf/etc/portage/make.profile
    ln -s /usr/portage/profiles/default/linux/arm/13.0 /usr/armv6j-hardfloat-linux-gnueabihf/etc/portage/make.profile
    armv6j-hardfloat-linux-gnueabihf-emerge -av xcb-proto
    armv6j-hardfloat-linux-gnueabihf-emerge -av libXt libXtst libXinerama

    # Install i686 Linux toolchain (amd64 hosts only)
    crossdev -v --target i686-pc-linux-gnu
    rm /usr/i686-pc-linux-gnu/etc/portage/make.profile
    ln -s /usr/portage/profiles/default/linux/x86/13.0 /usr/i686-pc-linux-gnu/etc/portage/make.profile
    i686-pc-linux-gnu-emerge -av xcb-proto
    i686-pc-linux-gnu-emerge -av libXt libXtst libXinerama

    # Install x86_64 Linux toolchain (x86 hosts only)
    crossdev -v --target x86_64-pc-linux-gnu
    rm /usr/x86_64-pc-linux-gnu/etc/portage/make.profile
    ln -s /usr/portage/profiles/default/linux/amd64/13.0 /usr/x86_64-pc-linux-gnu/etc/portage/make.profile
    x86_64-pc-linux-gnu-emerge -av xcb-proto
    x86_64-pc-linux-gnu-emerge -av libXt libXtst libXinerama

After installing the desired cross-toolchains, you can use the [devops](https://github.com/kwhat/jnativehook/tree/devops) branch for a collection of platform properties to build for. As of version 2.0, `ant -buildfile package.xml` should produce a complete distribution zip.  For 1.X, a small shell script is included to automate the process.

## Building
Extract source archive to a folder and run `ant all` in that folder to build a jar file.  There are several additional ant tasks available.  Run `ant -p` to display all targets along with a brief description of what they do.  If you would like to compile JNativeHook using LLVM's clang run `ant -Dant.build.native.toolchain=clang all`.

## Advanced Properties
Several optional properties exist for customizing the build process.

| Property                   | Type    | Default      | Description
|----------------------------|---------|--------------|------------
| ant.build.debug            | Boolean | false        | Enable debug build.
| ant.build.javac.compiler   | Enum    | modern       | Alias for Ant's build.compiler property. (classic, modern, jikes, jvc, kjc, gcj, sj)
| ant.build.javac.source     | Enum    | 1.5          | Ant property for source compatibility. (1.5, 1.6, 1.7, 1.8, ~~X.Y~~)
| ant.build.javac.target     | Enum    | 1.5          | Ant property for bytecode compatibility. (1.5, 1.6, 1.7, 1.8, ~~X.Y~~)
| ant.build.javac.args       | String  |              | Extra command-line arguments to pass to javac.
| ant.build.native.cflags    | String  | -O2 -pipe    | Flags to pass to the native compiler.
| ant.build.native.ldflags   | String  |              | Flags to pass to the native linker.
| ant.build.native.toolchain | Enum    | gcc          | The native toolchain to use.  (gcc, clang)
| ant.build.native.jobs      | Integer | auto         | The number of parallel jobs to run when compiling native code.
| ant.build.javac.include    | String  |              | Folder that contains the jni.h native header.  This value is normally auto-detected based on JDK/JAVA_HOME environment variable.
| ant.build.native.arch      | String  |              | Name of the native target architecture used for organizational purposes. This value is normally auto-populated based on Ant's os.arch property. (x86, x86_64, ppc, ppc64, arm, ~~alpha~~, ~~ia64_32~~, ~~ia64~~, ~~sparc~~)
| ant.build.native.os        | String  |              | Name of the native target operating system used for organizational purposes. This value is normally auto-populated based on Ant's OS family and name detection. (windows, linux, darwin, ~~solaris~~, ~~freebsd~~)
| ant.build.native.build     | String  |              | Native toolchain build prefix. This value is only used for cross compiler support.
| ant.build.native.target    | String  |              | Native toolchain build target. This value is only used for cross compiler support.

## Installation
The JNativeHook library ships with all the supported native libraries included in the jar file.  The library will attempt to identify the host operating system and extract the appropriate library to the system's temporary directory and dynamically load it.  If you need to extract the library to an alternative location, override the Java property "java.io.tmpdir" with a new temporary path.  For most users, simply placing the jar file in the program's class path is all that is required.

There may be specific circumstances where bundling the native library within the jar is undesirable.  For these situations the native library can be loaded from the `java.library.path` by specifying the location of the binary file.  For example, `java -Djava.library.path=/usr/lib -jar JNativeHook.jar`.  If both an external and internal library are present, the external library takes precedence.

For situations where custom library acquisition is required, the `jnativehook.lib.locator` property can be used to specify a class implementing `org.jnativehook.NativeLibraryLocator`.  For example, `java -Djnativehook.lib.locator=org.jnativehook.DefaultLibraryLocator -jar JNativeHook.jar`.