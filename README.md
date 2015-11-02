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

In addition to global keyboard and mouse events, the following system
properties are exposed by the native library.  Please note that there is no
guarantee that any of these properties will be available.
* jnativehook.key.repeat.rate
* jnativehook.key.repeat.delay
* jnativehook.button.multiclick.iterval
* jnativehook.pointer.sensitivity
* jnativehook.pointer.acceleration.multiplier
* jnativehook.pointer.acceleration.threshold
 
## Download
Binary distribution for JNativeHook can be found at the 
[releases](https://github.com/kwhat/jnativehook/releases) section of the 
[project page](https://github.com/kwhat/jnativehook/).  Maven users can 
use the artifact id JNativeHook with the group id com.1stleg to include 
this library automatically.  The nightly builds can be found at the maven 
[snapshot repository](https://oss.sonatype.org/content/repositories/snapshots/com/1stleg/jnativehook/2.0.SNAPSHOT/).

## Software and Hardware Requirements
* Java 1.5 - 1.8
* 256 MB of RAM
* Apple OS X 10.5 - 10.10
  * i586, amd64
  * Enable Access for Assistive Devices
* Windows 2000 - 10
  * i586, amd64
* X11 Linux
  * i586, amd64, arm6j
  * libX11.so.6
  * libXt.so.6
  * libXtst.so.6
  * libXext.so.6
  * libXdmcp.so.6
  * libXau.so.6
  * libICE.so.6
  * libSM.so.6
  * libxcb.so.1
  * libc.so.6
  * libdl.so.2
  * libuuid.so.1
  * libXinerama.so.1

## Donate
If you have found this library useful, please consider making a donation.
[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=UPMHT4ZFBTCBL)

## Compiling Instructions
Detailed instructions to [compile from source](wiki/Compile.md) are available in the wiki.

## Demo Applicaiton
The graphical example application was created to provided a real-time
demonstration of raw output for all available native events.  To run the
application simply execute the jar file provided. Source code available at 
[src/java/org/jnativehook/example/NativeHookDemo.java](src/java/org/jnativehook/example/NativeHookDemo.java).

## Example Code
* [Global Keyboard Listener](wiki/examples/Keyboard.md)
* [Global Mouse Listener](wiki/examples/Mouse.md)
* [Global Mouse Wheel Listener](wiki/examples/MouseWheel.md)
* [Working with Swing](wiki/examples/Swing.md)
* [Logging and Console Output](wiki/examples/ConsoleOutput.md)
* [Advanced Library Loading](wiki/examples/LibraryLoading.md)
* [Consuming Events (Unsupported)](wiki/examples/ConsoleOutput.md)

## Additional Information
Up to date source code and documentation available at:
[https://github.com/kwhat/jnativehook/](https://github.com/kwhat/jnativehook/)
