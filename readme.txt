*** About ***
JNativeHook is a library to provide global keyboard and mouse hooking for Java.
The primary purpose is to allow Java applacations access to keyboard and mouse 
events that would normally be outside of the scope of the running applcaiton.  

Additianlly information about keyboard  

Keyboard Events:
	Capture native key events for system keycodes between 0-255.
	System key codes will be translated to their virtual key code equivelents. 
	Events will be delivered to java for system keycodes between 0-255.

Keyboard Information:
	Keyboard repeate rate and the keybaoard repeate delay in miliseconds are 
	avaiable via their respective function calls.  

Mouse Button Events:
	Native mouse button events will be capture for mouse buttons 1-5.
	Mouse buttons will be translated to their equivalent virtual button code.
	The virutal button code will be delivered to Java.
	
Mouse Motion Events:
	Native mouse motion events will be captured for all mouse movements.
	Motion events will be returend to Java.

GUI Demo Applcation:
	Realtime dominstration of raw output for keyboard, button and mouse move
	events.

*** Software and Hardware Requirements ***
	Java 1.5 or later

	Linux / BSD Unix
		i586 or amd64
		X11 6.4 or Later with XRecord
		64 MB of RAM
		
	Mac OS X
		Apple Mac OS X 10.4 or Later
		i586, amd64, ppc or ppc64
		256 MB of RAM

	Solaris OS
		Oracle Solaris 8 or later
		i586, amd64, SPARC 32 or SPARC 64
		64 MB of RAM

	Windows 
		Microsoft Windows 2000 or later
		i586 or amd64
		32-bit 64 MB of RAM
		64-bit 128 MB of RAM


*** License ***
Copyright (c) 2006-2010 Alexander Barker. All Rights Received.
 
JNativeHook is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


*** Compiling ***
JNativeHook should compile on all operating systems that support Java.  Unix 
and Linux based systems will require the X Window System for compilation and 
execution. 

Unix/Linux:
	Dependencies:
		Ant 1.7 or later
		Java JDK 1.5 or later
		XOrg Server 1.8 or later
		XOrg Server development headers
		GCC 4.0 or later
		Make 3.8 or later
		
	Setup:
		Use your package manager to install all dependencies.


Windows:
	Dependencies:
		Ant 1.7 or later
		Java JDK 1.5 or later
		Windows x86:
			MinGW 5.0 or later
				MinGW base tools
				MinGW Make
		Windows x86_64
			The latest mingw-w64-bin_i686
		
	Setup:
		Install MinGW, Java JDK and Ant to a locations that do not include 
		spaces in the path.  For example:
			C:\MinGW
			C:\Java\jdk1.6.0_20
			C:\Java\jre6
			C:\ant\apache-ant-1.8.1
		
		After installing all the dependencies you will need to add a few 
		environment variables.  Open the System Properties dialog and navigate 
		to the Advanced tab.  Click the Environment Variables button and then 
		locate the Environment Variables dialog.  Click the new button to add 
		the following variables and their respective locations:
			ANT_HOME=C:\Ant\apache-ant-1.8.1
			JAVA_HOME=C:\Java\jdk1.6.0_20
		
		Now edit the Path variable and append the following:
			C:\MinGW\bin;%ANT_HOME%\bin;%JAVA_HOME%\bin


Mac OS X:
	Dependencies:
		Java 1.5 or Later
		Xcode Developer Tools 2.2.1 or later

	Setup:
		Create an account at http://connect.apple.com if you do not already 
		have one.  Now download and Install Xcode Tools.  Make sure you check 
		the box to install "UNIX Dev Support" which includes Ant, GCC and Make.


Building:
	Extract source archive to a folder without spaces and run ant in that 
	folder.  If custom build configuration is needed use the "configure" task 
	to generate a build.properties file for modification.


*** Installation ***
The JNativeHook library ships with all the supported native libraries included 
in the jar file.  The library will attempt to identify the host operating 
system and extract the appropriate library to a temporary location and 
dynamically load it.  All that is needed for most installations is placing the 
jar file in the programs class path.

There maybe specific circumstances where bundling the native library within the 
jar is undesirable.  For these situations the native library can be loaded from 
the java.library.path by specifying the location of the binary file.  For 
example java -Djava.library.path=/usr/lib -jar JNativeHook.jar.  If both an 
external and internal library are present the external library takes precedence.


*** Known Issues ***
The current native key code to virtual key code conversion method will probably 
only work for standard united states keyboard layout.  A more effective solution 
using a native key code to key string lookup should be implemented against the 
current keyboard layout and then translated to a virtual key code.
