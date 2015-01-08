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

## Software and Hardware Requirements
* Java 1.5 - 1.8
* 256 MB of RAM
* Apple OS X 10.5 - 10.10
  * i586, amd64
  * Enable Access for Assistive Devices
* Windows 2000 - 8.1
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

## Demo Applicaiton
The graphical example application was created to provided a real-time
demonstration of raw output for all available native events.  To run the
application simply execute the jar file provided.

## Sample Code
* [Global Keyboard Listener](https://github.com/kwhat/jnativehook/wiki/Usage#global-keyboard-listener)
* [Global Mouse Listener](https://github.com/kwhat/jnativehook/wiki/Usage#global-mouse-listener)
* [Global Mouse Wheel Listener](https://github.com/kwhat/jnativehook/wiki/Usage#global-mouse-wheel-listener)
* [Working with Swing](https://github.com/kwhat/jnativehook/wiki/Usage#working-with-swing)
* [Logging and Console Output](https://github.com/kwhat/jnativehook/wiki/Usage#logging-and-console-output)
* [Demo Application](https://github.com/kwhat/jnativehook/wiki/Usage#demo-application)
* [Consuming Events (Unsupported)](https://github.com/kwhat/jnativehook/wiki/Usage#consuming-events-unsupported)

## Donate
If you have found this library useful, please consider making a [donation](https://github.com/kwhat/jnativehook/wiki/Donate).

## Compiling Instructions
https://github.com/kwhat/jnativehook/wiki/Compiling

## Additional Information
Up to date source code and documentation available at:
[https://github.com/kwhat/jnativehook/](https://github.com/kwhat/jnativehook/)
