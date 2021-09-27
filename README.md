JNativeHook: Global keyboard and mouse listeners for Java.
==========================================================

[![Maven Central](https://maven-badges.herokuapp.com/maven-central/com.github.kwhat/jnativehook/badge.svg)](https://maven-badges.herokuapp.com/maven-central/com.github.kwhat/jnativehook)
![Nightly Build](https://github.com/kwhat/jnativehook/workflows/Nightly%20Build/badge.svg)

## About
JNativeHook is a library to provide global keyboard and mouse listeners for Java.  This will allow you to listen for
global shortcuts or mouse motion that would otherwise be impossible using pure Java.  To accomplish this task,
JNativeHook leverages platform-dependent native code through Java's native interface to create low-level system-wide
hooks and deliver those events to your application.

The following events are available via their respective listeners.
* Key Press Events
* Key Release Events
* Key Typed Events
* Mouse Down Events
* Mouse Up Events
* Mouse Click Events
* Mouse Move Events
* Mouse Drag Events
* Mouse Wheel Events

In addition to global event listeners, this library has the ability to post native events back to the native operating
system.

## Licensing
JNativeHook is covered under the [GNU Lesser General Public License](COPYING.LESSER.md) which is an extension of the
[GNU General Public License](COPYING.md) that grants you additional distribution rights.  As I understand it, you may
use this library in proprietary (closed source) projects so long as it remains an external library with the same binary
interface.  You may modify the source code of this library to fulfill any proprietary need, as long as those
modifications are made available under the terms and conditions of the LGPL.  Please consult with a licenced attorney
if you have additional licence compatibility questions or concerns.

## Download
Binary distribution for JNativeHook can be found at the
[releases](https://github.com/kwhat/jnativehook/releases) section of the
[project page](https://github.com/kwhat/jnativehook).  Maven users can
use the `<groupId>com.github.kwhat</groupId>` and `<artifactId>jnativehook</artifactId>`
to include this library automatically.  The nightly builds can be found at the maven
[snapshot repository](https://oss.sonatype.org/content/repositories/snapshots/com/github/kwhat/jnativehook/2.2-SNAPSHOT).

## Software and Hardware Requirements
* Java 1.8 - 17
* 256 MB of RAM
* Apple OS X 10.5 - 10.15
    * amd64, aarch64
    * Enable Access for Assistive Devices
* Windows 2000 - 10
    * i586, amd64, arm7
* X11 Linux
    * i586, amd64, arm7, aarch64
    * libxkbfile.so.1
    * libxkbcommon-x11.so.0
    * libxkbcommon.so.0
    * libX11-xcb.so.1
    * libX11.so.6
    * libxcb.so.1
    * libXinerama.so.1
    * libXt.so.6
    * libXtst.so.6
    * libc.so.6
    * libxcb-xkb.so.1
    * libdl.so.2
    * libXau.so.6
    * libXdmcp.so.6
    * libXext.so.6
    * libSM.so.6
    * libICE.so.6
    * libbsd.so.0
    * libuuid.so.1
    * librt.so.1
    * libpthread.so.0

## Donate
If you have found this library useful, please consider making a donation.
[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPMHT4ZFBTCBL)

## Documentation
* [Global Keyboard Listener](doc/Keyboard.md)
* [Global Mouse Listener](doc/Mouse.md)
* [Global Mouse Wheel Listener](doc/MouseWheel.md)
* [Thread Safety for AWT, Swing and JavaFX](doc/Swing.md)
* [Logging and Console Output](doc/ConsoleOutput.md)
* [Advanced Library Loading](doc/LibraryLoading.md)
* [Consuming Events (Unsupported)](doc/ConsumingEvents.md)

## Demo Application
The graphical example application exists to provide a real-time demonstration of raw output for all available native
events.  To run the application simply execute the jar file provided. Source code available at
[src/main/java/com/github/kwhat/jnativehook/example/NativeHookDemo.java](src/main/java/com/github/kwhat/jnativehook/example/NativeHookDemo.java).

## Additional Information
Up-to-date source code and documentation available at:
[https://github.com/kwhat/jnativehook/](https://github.com/kwhat/jnativehook/)
