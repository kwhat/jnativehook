*** About ***
JNativeHook is a library to provide global keyboard and mouse hooking for Java.
The primary purpose is to provide a portable and reliable method for delivering 
keyboard and mouse events to a Java application that would otherwise be 
inaccessible.  This is beneficial for applcatiions that run in the background 
but may require user interaction though hotkeys or mouse events. 

*** Breif Overview ***
Keyboard Events:
    Key event system codes and their modifier masks are captured at the native 
    level. The acquired system codes are then translated to their respective 
    key codes.  The key codes are then translated to virtual key codes, as 
    specified in the application programming interface.  Modifier masks are 
    translated directly from their native masks to virtual masks.  After all 
    native translations have taken place,the virtual key code and modifier mask 
    are delivered to Java via a NativeKeyEvent object.

Keyboard Information:
    Keyboard repeat rate and the repeat delay in milliseconds are provided via 
    their respective function calls.

Mouse Button Events:
    Mouse button events and their modifier masks are captured at the native 
    level.  The acquired button codes are then translated to virtual button 
	codes as specified in the application programming interface. Modifier masks 
	are translated directly from their native masks to virtual masks.  After 
	all native translations have taken place, the virtual button code and 
	modifier mask are delivered to Java via a NativeMouseEvent object.

Mouse Motion Events:
    Mouse motion events and their modifier masks are captured at the native 
    level.  The acquired pointer coordinates are untranslated from their native
	position.  The coordinates are usually relative to the upper left corner of 
	the native display, however that origin cannot be guaranteed.  Modifier 
	masks are translated directly from their native masks to virtual masks.  
	After all native translations have taken place, the virtual key code and 
	modifier mask are delivered to Java via a NativeMouseEvent object.

GUI Demo Application:
    The graphical example application was created to provided a real-time
    demonstration of raw output for keyboard, button and mouse motion events.  
    To run the application simply exceute the jar file provided.

*** Software and Hardware Requirements ***
	Java 1.5 or 1.6

	Linux
		i586 or amd64
		X11 6.4 to 7.4 with working XRecord
		64 MB of RAM

	Mac OS X
		Apple Mac OS X 10.4 to 10.6
		i586, amd64, ppc or ppc64
		256 MB of RAM

	Windows 
		Microsoft Windows 2000 to 7
		i586 or amd64
		32-bit 64 MB of RAM
		64-bit 128 MB of RAM


*** License ***
Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Alternate Licensing Options

In addition to the GNU General Public License, I offer business-friendly 
licensing options for JNativeHook.  If you wish to incorporate JNativeHook 
into your commercial software package where the terms of the GPL are not a 
viable option, you may choose to obtain a commercial license.  For more 
information on available licensing options, please contact me via email at 
alex@1stleg.com or by paper mail at the following address:

Alexander Barker
3946 Verde Vista Dr.
Thousand Oaks CA 91360 


*** Compiling ***
JNativeHook should compile on all operating systems that support Java.  Unix 
and Linux based systems will require the X Window System for compilation and 
execution.

Unix/Linux:
	Dependencies:
		Ant 1.7 or 1.8
		Java JDK 1.5 or 1.6
		libXtst 1.1.0
		libX11 1.3.4
		GCC 4.1, 4.2, 4.3, 4.4 or 4.5
		Make 3.80, 3.81 or 3.82
		
	Setup:
		Use your package manager to install all dependencies.

Mac OS X:
	Dependencies:
		Java 1.5 or 1.6
		Xcode Developer Tools 2.2.1

	Setup:
		Create an account at http://connect.apple.com if you do not already 
		have one.  Now download and Install Xcode Tools.  Make sure you check 
		the box to install "UNIX Dev Support" which includes Ant, GCC and Make.

Windows:
	Dependencies:
		Ant 1.7 or 1.8
		Java JDK 1.5 or 1.6
		Windows x86:
			MinGW 5.0
				MinGW base tools
				MinGW Make
		Windows x86_64
			The latest stable mingw-w64-bin_i686
		
	Setup:
		Install MinGW, Java JDK and Ant to a locations that do not include 
		spaces in the path.  For example:
			C:\MinGW
			C:\Java\jdk1.6.0_20
			C:\Java\jre6
			C:\ant\apache-ant-1.8.1
		
		After installing all the dependencies, you will need to add a few 
		environment variables.  Open the System Properties dialog and navigate 
		to the Advanced tab.  Click the Environment Variables button and then 
		locate the Environment Variables dialog.  Click the new button to add 
		the following variables and their respective locations:
			ANT_HOME=C:\Ant\apache-ant-1.8.1
			JAVA_HOME=C:\Java\jdk1.6.0_20
		
		Now edit the Path variable and append the following:
			C:\MinGW\bin;%ANT_HOME%\bin;%JAVA_HOME%\bin


Building:
	Extract source archive to a folder without spaces and run Ant in that 
	folder.  If custom build configuration is needed, use the "configure" task 
	to generate a build.properties file for modification.


*** Installation ***
The JNativeHook library ships with all the supported native libraries included 
in the jar file.  The library will attempt to identify the host operating 
system and extract the appropriate library to a temporary location and 
dynamically load it.  All that is needed for most installations is placing the 
jar file in the programs class path.

There may be specific circumstances where bundling the native library within the 
jar is undesirable.  For these situations the native library can be loaded from 
the java.library.path by specifying the location of the binary file.  For 
example, java -Djava.library.path=/usr/lib -jar JNativeHook.jar.  If both an 
external and internal library are present, the external library takes precedence.


*** Known Issues ***
The current native key code to virtual key code conversion method may fail for 
some non-English keys.  If you encounter an incorrect key code, please report 
the issue and include the output of the NativeKeyEvent's paramString().


*** Solaris and BSD Support ***
This program should compile and run on both Oralce Solaris and BSD Unix.  No 
binaries are provided for these platforms at this time due to cross compiler 
difficulties and the lack of official Java support on the BSD platform.  Please 
feel free to build on these platforms and report any issues that may arise.
