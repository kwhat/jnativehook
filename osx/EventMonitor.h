#include <Carbon/Carbon.h>

// Radio box control ID defines to access control items in the "Main Window"
enum {
	kCmdKeyDown			= 'KYDN',
	kCmdKeyUp			= 'KYUP',
	kCmdKeyRepeat			= 'KYRP',
	kCmdKeyModifiersChanged		= 'KYMC',
	
	kCmdMouseDown			= 'MODN',
	kCmdMouseUp			= 'MOUP',
	kCmdMouseMoved			= 'MOMO',
	kCmdMouseDragged		= 'MODR',
	kCmdMouseWheel			= 'MOWH',
	
	kCmdTabletPoint			= 'TBPT',
	kCmdTabletProximity		= 'TBPR',
	
	kCmdForeground			= 'FORE'	// process foreground event just like background events.
};

extern OSStatus InstallMyEventHandlers(void);

