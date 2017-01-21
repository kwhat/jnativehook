[![Maven Central](https://maven-badges.herokuapp.com/maven-central/com.1stleg/jnativehook/badge.svg)](https://maven-badges.herokuapp.com/maven-central/com.1stleg/jnativehook)

## About
JNativeHook is a library to provide global keyboard and mouse listeners for
Java.  This will allow you to listen for global shortcuts or mouse motion that
would otherwise be impossible using pure Java.  To accomplish this task,
JNativeHook leverages platform-dependent native code through Java's native
interface to create low-level system-wide hooks and deliver those events to
your application.

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

In addition to global event listners, this library has the ability to post native events back to the native operating 
system.
 
## Download
Binary distribution for JNativeHook can be found at the 
[releases](https://github.com/kwhat/jnativehook/releases) section of the 
[project page](https://github.com/kwhat/jnativehook/).  Maven users can 
use the `<groupId>com.1stleg</groupId>` and `<artifactId>jnativehook</artifactId>` 
to include this library automatically.  The nightly builds can be found at the maven 
[snapshot repository](https://oss.sonatype.org/content/repositories/snapshots/com/1stleg/jnativehook/1.1.SNAPSHOT/).

## Software and Hardware Requirements
* Java 1.5 - 1.8
* 256 MB of RAM
* Apple OS X 10.5 - 10.8
  * i586, amd64
  * Enable Access for Assistive Devices
* Windows 2000 - 7
  * i586, amd64
* X11 Linux
  * i586, amd64, arm6j
  * libX11.so.6
  * libXt.so.6
  * libXtst.so.6
  * libSM.so.6
  * libICE.so.6
  * libXext.so.6
  * libXdmcp.so.6
  * libXau.so.6
  * libxcb.so.1
  * libc.so.6
  * libdl.so.2
  * libpthread.so.0

## Donate
If you have found this library useful, please consider making a donation.
[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPMHT4ZFBTCBL)

## Compiling Instructions
Detailed instructions to [compile from source](https://github.com/kwhat/jnativehook/wiki/Compiling) are available in the wiki.

## Demo Applicaiton
The graphical example application was created to provided a real-time
demonstration of raw output for all available native events.  To run the
application simply execute the jar file provided. Source code available at 
[src/java/org/jnativehook/example/NativeHookDemo.java](src/java/org/jnativehook/example/NativeHookDemo.java).

## Example Code
* [Global Keyboard Listener](https://github.com/kwhat/jnativehook/wiki/Keyboard)
* [Global Mouse Listener](https://github.com/kwhat/jnativehook/wiki/Mouse)
* [Global Mouse Wheel Listener](https://github.com/kwhat/jnativehook/wiki/MouseWheel)
* [Working with Swing](https://github.com/kwhat/jnativehook/wiki/Swing)
* [Logging and Console Output](https://github.com/kwhat/jnativehook/wiki/ConsoleOutput)
* [Advanced Library Loading](https://github.com/kwhat/jnativehook/wiki/LibraryLoading)
* [Consuming Events (Unsupported)](https://github.com/kwhat/jnativehook/wiki/ConsoleOutput)

## Additional Information
Up to date source code and documentation available at:
[https://github.com/kwhat/jnativehook/](https://github.com/kwhat/jnativehook/)
