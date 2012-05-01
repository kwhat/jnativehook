*** About ***
JNativeHook is a library to provide global keyboard and mouse listeners for
Java. The library aims to provide a portable and reliable method for delivering
keyboard and mouse events to a Java application that would otherwise be
inaccessible. This will beneficial for any Java application that requires user
input without having focus. The library accomplishes this task by leveraging
platform dependent native code to create a low level hook and deliver native
events back to the virtual machine through Java's native interface.

*** Software and Hardware Requirements ***
	Linux
		i586 or amd64
		64 MB of RAM
		Java 1.5 - 1.7
		x11-libs/libX11 1.0 - 1.4
		x11-libs/libXt 1.0 - 1.1
		x11-libs/libXtst 1.0 - 1.2

	Mac OS X
		i586, amd64, powerpc or powerpc64
		256 MB of RAM
		Apple Mac OS X 10.5 - 10.7

	Windows
		i586 or amd64
		32-bit 64 MB of RAM
		64-bit 128 MB of RAM
		Microsoft Windows 2000 - 7
		Java 1.5 - 1.7

*** Sample Code ***
The graphical example application was created to provided a real-time
demonstration of raw output for keyboard, button and mouse motion events.  To
run the application simply execute the jar file provided.

*** Additional Information ***
Up to date source code and documentation avaiable at:
http://code.google.com/p/jnativehook/