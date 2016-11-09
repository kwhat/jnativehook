## ChangeLog

**JNativeHook 2.1.0 (Pending)**
 * All events now use the native time source instead of the Unix epoch.
 * Added adapter classes.
 * Added native monitor properties.
 * Added method calls for global properties.
 * Added support for keyboard lock masks.
 * Added support for Swing event conversion.
 * Relaxed GlobalScreen scope restrictions.
 * Fixed a major issue with UTF-16 support on Linux.
 * Fixed key mapping for the numberpad on Linux.
 * Fixed language support on X11 platforms.
 * Code refactroing and orginization.
 
**JNativeHook 2.0.3 (Nov 02, 2015)**
 * Added support for media keys on OS X.
 * Added wiki pages to the project.
 * Fixed caps-lock key release on OS X.
 * Fixed posting mouse coordinates on Windows and Linux.
 * Fixed posting number keys on Windows.
 * Fixed segfault with multiple instances on Linux.
 * Fixed SetWindowsHookEx failed with 0X7E error on Windows.
 * Fixed the coordinates of posted mouse events for Windows. 
 * Small documentation updates.

**JNativeHook 2.0.2 (Jun 28, 2015)**
 * Added support for Maven central repository.
 * Added modifier polling to hook start procedure.
 * Fixed a bug on OS X where the hook would always succeed.
 * Fixed event timing issues for all platforms.
 * Fixed issue with build.property file being included in the release package.
 * Various cleanups throughout the code base.

**JNativeHook 2.0.1 (Mar 21, 2015)**
 * Added support for dispatch_sync_f when available on OS X.
 * Added libuiohook as a git submodule.
 * Changed hard coded dlsym() paths to RTLD_DEFAULT on OS X.
 * Fixed a memory leak in event posting on OS X.
 * Fixed an LTO related crash on Linux.
 * Fixed missing button number for mouse pressed events on Windows.

**JNativeHook 2.0.0 (Jan 27, 2015)**
 * Total rewrite and fork of the native code to its own [standalone project](https://github.com/kwhat/libuiohook).
 * Transitioned to virutal keycodes backed by scancode set 1.
 * Added Java's ExecutorService support for event delivery.
 * Added the ability to post NativeInputEvent's back to the Operating System.
 * Added support for multi-head hosts.
 * Countless bug fixes and perfomance improvments.
 * Fixed a long-standing RDP capture issue.
 * More flexible library loading.
 * More accurate event timestamps.
 * Removed the vestigial singleton API.
 * Vastly improved the build process.
 
**JNativeHook 1.1.4 (Mar 17, 2013)**
 * Added no-strict-aliasing compiler option that is enabled by default for all platforms.
 * Fixed 100% CPU utilization on Linux when using asynchronous XRecord API.
 * Fixed a crash when re-registering the native hook on all platforms.
 * Fixed the dead key interpretation for key typed events on Windows.
 * Improved language support for multiple locales on Windows.
 * Improved the memory utilization for the demo application.
 * Updated the demo application to allow unregistering and re-registering of the global hook.
 * Various cleanups throughout the native code base.

**JNativeHook 1.1.3 (Jan 2, 2013)**
 * Fixed a JVM memory leak due to NativeInputEvent objects not being garbage collected.
 * Fixed exception for assistive devices on OS X.
 * Fixed JNLP and custom class loading issues in native code.
 * Fixed mouse click events on Windows.
 * Fixed key typed events for OS X, again.  This should prevent the TSMProcessRawKeyCode failed (-192) errors from popping up.
 * Fixed key typed events for tab, enter and number pad keys on X11 platforms.
 * Improved key char lookup for OSX.
 * Updated the author and license of the code used in WinUnicodeHelper.

**JNativeHook 1.1.2 (Nov 5, 2012)**
 * Added asynchronous XRecord build option and enabled it by default for Unix.
 * Fixed the click count for OS X.
 * Fixed key character lookup including support for dead keys on OS X.
 * Improved stability for synchronous XRecord on Unix.
 * Improved thread startup and shutdown across all platforms.
 * Improved thread safety for Unix and OS X.
 * Various cleanups throughout the native code base.

**JNativeHook 1.1.1 (Oct 17, 2012)**
 * Added an explicit exception if the native library was not found.
 * Added safety checks to the Windows unicode helper function.
 * Changed to a listen only tap for OS X.
 * Fixed a few small memory leaks for OS X.
 * Fixed click count for OS X.
 * Fixed free() on static memory pointer for Unix and OS X.
 * Fixed potential sscanf overflow on Unix.
 * Fixed potential use of a null pointer in rare circumstances for Unix and OS X.
 * Fixed several invalid unicode conversions for Unix.
 * Fixed ant run target java library path.
 * Improved ant platform compatibility.
 * Removed unneeded ActionListener from the NativeHookDemo.
 * Various cleanups throughout the native code base.

**JNativeHook 1.1.0 (Sep 10, 2012)**
 * Total rewrite and restructuring of the native code base.
 * Added support for key typed events.
 * Added support for action key checks.
 * Added support for mouse click events.
 * Added support for mouse drag events.
 * Added support for mouse wheel events.
 * Improved both native and Java thread safety.
 * Improved unpacking of the native library from the Jar.
 * Improved operating system compatibility.
 * Optimized native code for all platforms.
 * Removed Make dependency.
 * Workaround for Java web start shortfalls.

**JNativeHook 1.0.2288 (Sep 23, 2011)**
 * Added button masks to all platforms for consistency.
 * Added checks for null on GlobalScreen listener addition and removal.
 * Added WM_QUIT exit listener instead of isRunning boolean for Windows message loop.
 * Changed Carbon constant for the Context Menu to a user defined value in OSXKeyCodes.h.
 * Enabled sun.awt.enableExtraMouseButtons by default.
 * Fixed an issue with Linux mouse buttons 4 and 5.  Switched to using the standard virtual button codes for events.
 * Fixed various typos.
 * Fixed the termination order for Unix/Linux XRecord connection.
 * Fixed an issue with the MouseEvent constructor id for all platforms. Separate id's are required for button and motion events.
 * Fixed the double event registration issue with the demo.
 * Fixed issue with Makefile error message not displaying if run outside of ant.
 * Fixed issues with null pointer VM crashes due to garbage collection for all platforms.
 * Fixed RunLoop termination on OS X.
 * Fixed issues with the OS X button modifiers. 
 * Improved MinGW compatibility Windows.
 * Improved event delivery performance for all platforms.
 * Removed Carbon code from the keyboard rate and delay functions on OS X.
 * Removed the Carbon HIToolbox requirement for key constants on OS X.
 * Switch to async XRecord connection.
 * Switch from Carbon to AplicationServices framework for keyboard hooking on OS X.

**JNativeHook 1.0.1950 (Aug 30, 2011)**
 * Fixed a problem with the 32-bit OS X binary.
 * Fixed various typos.
 * Removed unneeded `-framework JavaVM` for OS X targets.
 * Improved build file.

**JNativeHook 1.0.1840 (May 11, 2011)**
 * Initial Release.
