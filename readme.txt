*** About ***
JNativeHook is a library to provide global keyboard and mouse listeners for 
Java.  This will allow you to listen for global shortcuts or mouse motion that 
would otherwise be impossible using pure Java.  To accomplish this task, 
JNativeHook leverages platform dependent native code through Java's native 
interface to create low level system wide hooks and deliver those events to 
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
  jnativehook.autoRepeatRate
  jnativehook.autoRepeatDelay
  jnativehook.multiClickInterval
  jnativehook.pointerSensitivity
  jnativehook.pointerAccelerationMultiplier
  jnativehook.pointerAccelerationThreshold

*** Software and Hardware Requirements ***
    Linux
        i586 or amd64
        64 MB of RAM
        Java 1.5 - 1.7
        x11-libs/libX11 1.0 - 1.5
        x11-libs/libXt 1.0 - 1.3
        x11-libs/libXtst 1.0 - 1.2
		x11-libs/libxcb 1.7 - 1.8

    Mac OS X
        i586, amd64, powerpc or powerpc64
        256 MB of RAM
        Java 1.5 - 1.7
        Apple Mac OS X 10.5 - 10.8

    Windows
        i586 or amd64
        32-bit 64 MB of RAM
        64-bit 128 MB of RAM
        Microsoft Windows 2000 - 7
        Java 1.5 - 1.7

*** Sample Code ***
The graphical example application was created to provided a real-time
demonstration of raw output for all available native events.  To run the 
application simply execute the jar file provided.

*** Additional Information ***
Up to date source code and documentation available at:
http://code.google.com/p/jnativehook/
