*** About ***
JNativeHook is a Java library to provide a global keyboard and mouse listening 
capabilities for applications regardless of focus.


*** License ***
Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 
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
		Make sure that you install MinGW, Java JDK and Ant to a location that 
		does not have spaces in the path.  For example:
			C:\MinGW
			C:\Java\jdk1.6.0_16
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
		Java 1.5 or later
		Xcode Developer Tools 2.2.1 or later

	Setup:
		Create an account at http://connect.apple.com if you do not already 
		have one.  Now download and Install Xcode Tools.  Make sure you check 
		the box to install "UNIX Dev Support" which includes Ant, GCC and Make.


Building:
	Extract source archive to a folder without spaces and run ant in that 
	folder.  If custom build configuration is needed use the "configure" task 
	to generate a build.properties file for modification.
