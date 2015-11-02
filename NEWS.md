## News

**JNativeHook 2.0.3 Maintenance Release (Nov 02, 2015)**

Maintenance release 3 addresses a few small issues across all supported platforms.  Please see the [ChangeLog](ChangeLog.md) for more information. Work has now started on version 2.1 of the library.  Future work will include better language support for X11 and possibly Wayland support.

**JNativeHook 2.0.2 Maintenance Release (Jun 28, 2015)**

Maintenance release 2 addresses a few small issues across all supported platforms.  The biggest change is the addition of this library to the maven central repository.  You should now be able to add JNativeHook to your project via the groupId=com.1stleg and artifactId=jnativehook.


**JNativeHook 2.0.1 Maintenance Release (Mar 21, 2015)**

This release addresses several issues on OS X including a memory leak.  Other bug fixes include a missing button number on windows and LTO related compiler issues.


**JNativeHook 2.0.0 Major Release (Jan 27, 2015)**

Version 2.0 is now officially available.  After reviewing the GlobalScreen API, I decided to make a few minor changes.  I have removed the GlobalScreen singleton and convert all object methods to static methods.  Additionally the `DefaultDispatchService` was externalized to its own public class and a new `SwingDispatchService` was added for those working with swing.  There is also an all new `NativeLibraryLocator` interface contributed by Aidas Adomkus that will allow you to override the default native library unpacking and loading.  You can specify an implementing class with the `jnativehook.lib.locator` property prior to the class being loaded.


**JNativeHook 2.0.0-RC5 General Preview (Jan 02, 2015)**

Version 1.2 has now been bumped to 2.0 ahead of the official release.  Sorry for the late version change.  I didn't feel like it was appropriate to keep this in the 1.X branch because it shares almost no code in common.  The fifth release candidate addresses two small issues that have resulted in some radical changes to the code base.  

The first was a NullPointerException on OS X targets when the accessibility API is disabled.  As best as I could tell, this exception was originating in the JVM or one of Apple's frameworks and not from the library itself.  I suspect it was related to the use of POSIX threads but I was unable to confirm the exact source of the exception.  I decided to resolve this issue by moving the native thread code into Java because I keep coming across POSIX thread compatibility issues in Apple's frameworks.  This has drastically simplified the native code and doesn't appear to have caused any additional issues, however, users should thoroughly test.  More information about this change is available in [Issue 27](https://github.com/kwhat/jnativehook/issues/27).

The second bug that was fixed was an issue with RDP, VMWare and other applications that somehow managed to disable the global hook for their context.  This was resolved by adding a Window listener for name changes and restarting the hook when one was received.  This will basically restart the hook when your mouse enters a new window.  If that sounds like an excessive amount of start and stops, it probably is, but thankfully the Microsoft API to start and stop hooks appears to be very fast.  Again, Windows users should test RC5 for performance if it is critical to their application.  

This will most likely be the final release candidate before the 2.0 final release.  Most of this period will best spent updating the documentation.  There are still two outstanding issues that I would like to address before the final release, however, I need some additional information on both.  If you have a Japanese keyboard or know how to type on one, can you please look at [Bug #58](https://code.google.com/p/jnativehook/issues/detail?id=58) and let me know if it is still a problem or what key combinations I need to use to reproduce the problem with a US English keyboard?  The second issue requires that you have a multimedia keyboard with keys like volume up and volume down.  Please test your multimedia keys and see what they report.  If they do not produce the correct keycode, please add an entry to [Bug #59](https://code.google.com/p/jnativehook/issues/detail?id=59) and tell me the operating system, keyboard type, key you pressed, key code, and the raw code it produced.  I would like to get version 2.0 wrapped up by February to free up some time to work on a new library.  Stay tuned for something exciting.


**JNativeHook 1.2.0-RC4 General Preview (Dec 02, 2014)**

This release addresses a problem with the native thread attaching to the JVM which was causing event delivery failure in Java.  Some smaller bugs were addressed in the keycode char look up for OS X and Windows.


**JNativeHook 1.2.0-RC3 General Preview (Nov 22, 2014)**

This release includes fixes for a couple of thread-related issues and a problem with mouse tracking on multi-head displays.  Java class targets were moved back to version 1.5.


**JNativeHook 1.2.0-RC2 General Preview (Oct 09, 2014)**

Release candidate #2 is now ready for download.  This release fixes several issues with exceptions thrown from native code.  The event timestamps were also modified to pin their source time to the native event source clock.  This will provide very accurate event timing while lowering the overhead on Windows and Linux platforms. Windows 32-bit targets should see the largest performance gain.  OS X environments will see only minimal improvement due to the resolution of available timers.  Please report any bugs to the GitHub bug tracker.


**JNativeHook 1.2.0-RC1 General Preview (Sep 14, 2014)**

After several delays and last minute bugs, the first release candidate for version 1.2 is finally ready for download.  This release candidate has focused heavily on threading and stability.  All users of version 1.1 should begin migrating their code to version 1.2 immediately.


**JNativeHook 1.2.0-Beta2 Developer Release (Apr 22, 2014)**

The second beta release of version 1.2 is new ready for testing.  You can find a link to the download [here](https://github.com/kwhat/jnativehook/releases).  Beta 2 includes lots of bug fixes in both [JNativeHook](https://github.com/kwhat/jnativehook) and its parent library [libuiohook](https://github.com/kwhat/libuiohook).  Beta 2 also includes support for compiling with the LLVM toolchain and now ships with Mac OS X Intel binaries!  Support for the PowerPC architecture has been suspended due to its age.  If you require PowerPC binary support, please file a bug report and plead your case.

All of the cross-compiler toolchain code used to build JNativeHook has now been released across several repositories attached to my [GitHub](https://github.com/kwhat) account.  These tools were designed to be used with [Gentoo's crossdev](https://www.gentoo.org/proj/en/base/embedded/cross-development.xml) however, the [binutils-apple](https://github.com/kwhat/binutils-apple) should be compatible with any Linux distribution that has clang 3.4 available.  To install on Gentoo, add the [crossdev-apple](https://github.com/kwhat/crossdev-apple-overlay) layman overlay to your system and emerge the patched version of [crossdev](https://github.com/kwhat/crossdev-apple) included in the overlay.  I will be adding more instructions for using these tools as time permits.  If you have questions about using these tools, please contact me directly.


**JNativeHook 1.2.0-Beta1 Developer Release (Jan 22, 2014)**

Version 1.2.0 is finally ready for testing after an architectural redesign to improve performance and stability.  The most notable of these changes are a split in the code base and the way virtual keys are handled.  The core hooking functionality of JNativeHook has been removed and a standalone C library called [libuiohook](https://github.com/kwhat/libuiohook) was created to provide the same functionality.  This will allow for debugging without having to deal with the JVM.  The virtual keys were also modified to closely follow set 1 scan codes to simplify native translations and provide a modest performance boost.

Binary downloads for Windows and Linux are available [here](https://github.com/kwhat/jnativehook/releases/tag/v1.2.0-Beta1).  Unfortunately it has become increasingly difficult to support a binary distribution targeting Apple's OS X.  Subsequently, no binaries will be provided for that platform at this time.  I am currently working on a couple of methods to work around these issues, however, it will be some time before I have a solution in place.  In the interim, OS X users are encouraged to compile JNativeHook on their local machine.  I will post updated instructions for compiling from source on the Git Hub wiki shortly.

Project migration and licence update.  Due to a number of reasons including, but not limited to, the removal of binary file hosting and functional git repository hosting, I have decided to abandon Google project hosting in favor of [GitHub](https://github.com/kwhat).  In addition to the hosting migration, I have also decided to update project licencing from GPLv3 to LGPLv3.  A Commercial licencing option will still be available for those that require it.


**JNativeHook 1.1.4 Maintenance Release (Mar 17, 2013)**

Version 1.1.4 is now ready for download. ([changelog](ChangeLog.md))  This version includes several bug fixes for Windows language support as well as a crash affecting all platforms.  The API is still 100% compatible with the earlier 1.1 releases.  Updating is recommended for everyone using the 1.1 branch.


**JNativeHook 1.1.3 Maintenance Release (Jan 2, 2013)**

Version 1.1.3 is now ready for download. ([changelog](ChangeLog.md))  This version includes several bug fixes for all supported platforms.  The API is still 100% compatible with the earlier 1.1 releases.  Updating is recommended for everyone using the 1.1 branch.


**Status Update 1.1.3, 1.2.0 and More? (Dec 21, 2012)**

Well, the world hasn't ended yet but there is a bit of news about the JNativeHook road map.  Development has been a bit slow lately due to the holidays and a few other fast approaching project deadlines that have been consuming most of my free time.  Version 1.1.3 will address the resurfacing key character problem on OS X ([bug #25](https://code.google.com/p/jnativehook/issues/detail?id=25) & [bug #30](https://code.google.com/p/jnativehook/issues/detail?id=30)) and the NoClassDefFoundError for all platforms.  The key character problem has finally been confirmed to be a thread safety issue with no help from Apple's API documentation.  I am currently looking into a few different methods of addressing the problem.  The NoClassDefFoundError has already been resolved in the [1.1 branch](https://github.com/kwhat/jnativehook/tree/1.1).  If you do not want to compile from source, a binary version that includes the fix is attached to [bug #28](https://code.google.com/p/jnativehook/issues/detail?id=28).  Version 1.2 is planned for shortly after the 1.1.3 release.  This  version brings a couple of exciting new features to the library.  The first is a more flexible implementation of Java's core Robots class for generating native keyboard and mouse events.  The second will bring JNativeHook to languages beyond Java by packaging an additional standalone native library that is independent of the JNI library already shipped with the jar.  Developers wishing to utilize the power and flexibility of JNativeHook without using Java will soon have the option to do so.


**JNativeHook 1.1.2 Maintenance Release (Nov 5, 2012)**

Version 1.1.2 is now ready for download. ([changelog](ChangeLog.md))  This version includes several bug fixes for OS X including a critical thread safety issue effecting all POSIX-based platforms. (See  [bug #25](https://code.google.com/p/jnativehook/issues/detail?id=25))  The API is still 100% compatible with the earlier 1.1 releases.  Updating is recommended for everyone using the 1.1 branch.


**JNativeHook 1.1.1 Maintenance Release (Oct 17, 2012)**

Version 1.1.1 is now ready for download. ([changelog](ChangeLog.md))  This version includes various bug fixes and code cleanup across all supported platforms.  The API has remained 100% compatible with the earlier 1.1.0 release.  Updating is recommended for everyone using the 1.1 branch.


**JNativeHook 1.1.0 Final Released (Sep 10, 2012)**

After almost a year in development, version 1.1.0 is finally ready for an official release. ([changelog](ChangeLog.md))  All known bugs should be fixed at this time.  Thank you to everyone who submitted reports and helped test over the last year.  If you have found this project helpful and would like to contribute, I am now taking [donate donations] to help cover development costs.  You may also contribute by visiting the Issues tab and submitting patches or information on open bugs.  If you have any project-related questions please feel free to email me or stop by the [IRC channel](http://webchat.freenode.net/?channels=#JNativeHook)  [#JNativeHook@chat.freenode.net](irc://chat.freenode.net/%23JNativeHook).


**JNativeHook 1.1.0 RC 3 Released (July 20, 2012)**

Due to the number of problems with release candidate 2, I have decided to put another release candidate together.  The current version should address all known bugs with the exception of a possible crash on OS X.  If you experience a crash please file a bug report and attach the jnh-error.txt or equivalent error log.


**JNativeHook 1.1.0 RC 2 Released (May 8, 2012)**

The second and probably last release candidate for version 1.1 is ready for download. ([changelog](ChangeLog.md))  Most of the work has gone into polishing up the code base and documentation. Several minor bug fixes have been addressed including another web start issue. There have been a few small API changes, mainly method names and modifiers, throughout the project. Please consult the documentation for the updated method signatures.


**JNativeHook 1.1.0 RC 1 Released (Apr 28, 2012)**

The first release candidate for version 1.1 has been released. ([changelog](ChangeLog.md)) OS X binaries should finally work. Major work has gone into producing accurate key typed events. This includes only firing key typed evens when a printable character is pressed and producing the correct native Unicode character for that event.


**JNativeHook 1.1.0 Beta 2 Released (Mar 31, 2012)**

The second beta for version 1.1 has been released. ([changelog](ChangeLog.md)) Fixed several cross compiler issues for OS X targets. These targets should now compile and run correctly. Added support for key typed and mouse click events. Additional native code restructuring and build file cleanup. All targets should function correctly at this point. Some erroneous events may be generated for key typed events at this time. These will be fixed in the near future.


**JNativeHook 1.1.0 Beta 1 Released (Mar 20, 2012)**

The first beta for version 1.1 has been released. ([changelog](ChangeLog.md)) This version is almost a complete rewrite of the original 1.0 release with a focus on exception recovery and thread safety. There are still a few issues to work out with Apple OSX system properties and Exception cleanup, but for the most part the core functionality is very stable.
