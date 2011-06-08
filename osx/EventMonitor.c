/* Compile:
 * gcc -framework Carbon EventMonitor.c -o EventMonitor
 * 
 */

#include <Carbon/Carbon.h>

static EventHandlerRef sHandler;

/*
	MonitorHandler - called whenever the Event monitor target detects a registered event happens.
	determine the type of event and send a message to the system log as to the event type
	gcc -framework Carbon test.c -o a.out
 */
static OSStatus MonitorHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon ) {
	char* className = NULL;
	char* kindName = NULL;
	Point	pt;
	UInt32	ch;
	EventMouseButton button;

	EventFlags event_mask = EventGetFlags(inEvent);
	printf("Native: EventGetFlags - 0x%X\n", event_mask);

	// get the event class
	switch ( GetEventClass( inEvent ) ) {
		case kEventClassKeyboard:
			className = "Keyboard";
			// get the keyboard event type
			switch ( GetEventKind( inEvent ) ) {
				case kEventRawKeyDown:
					kindName = "KeyDown";
					GetEventParameter(inEvent, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &ch);
					fprintf(stderr, "keycode - %d - ", (int)ch);
				break;
					
				case kEventRawKeyUp:
					kindName = "KeyUp";
					GetEventParameter(inEvent, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &ch);
					fprintf(stderr, "keycode - %d - ", (int)ch);
				break;
					
				case kEventRawKeyRepeat:
					kindName = "KeyRepeat";
					GetEventParameter(inEvent, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &ch);
					fprintf(stderr, "keycode - %d - ", (int)ch);
				break;
					
				case kEventRawKeyModifiersChanged:
					GetEventParameter(inEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &ch);
					fprintf(stderr, "0x%x - ", (unsigned int)ch);
					kindName = "ModifiersChanged";
				break;
					
				default:
				break;
			}
		break;
			
		case kEventClassMouse:
			className = "Mouse";
			// get the type or mouse event
			switch ( GetEventKind( inEvent ) ) {
				case kEventMouseDown:
					kindName = "MouseDown";
					GetEventParameter(inEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
					fprintf(stderr, "Mouse button %d pressed\n", (int) button);
				break;
					
				case kEventMouseUp:
					kindName = "MouseUp";
					GetEventParameter(inEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
					fprintf(stderr, "Mouse button %d released\n", (int) button);
				break;
					
				case kEventMouseMoved:
					kindName = "MouseMoved";
					GetEventParameter(inEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &pt);
					fprintf(stderr, "location - %d, %d - ", pt.v, pt.h);
				break;
					
				case kEventMouseDragged:
					kindName = "MouseDragged";
				break;
					
				case kEventMouseWheelMoved:
					kindName = "MouseWheel";
				break;
					
				default:
				break;
			}
		break;
			
		case kEventClassTablet:
			className = "Tablet";
			// get the kind of tablet event
			switch ( GetEventKind( inEvent ) ) {
				case kEventTabletPoint:
					kindName = "TabletPoint";
				break;
					
				case kEventTabletProximity:
					kindName = "TabletProximity";
				break;
					
				default:
				break;
			}
			break;
			
		default:
		break;
	}
	
	fprintf( stderr, "event class = %s, kind = %s\n", className != NULL ? className : "<unknown>", kindName != NULL ? kindName : "<unknown>" );
	
	return noErr;
}

OSStatus InstallMyEventHandlers(void) {
	EventTypeSpec	kEvents[] = {
		// use an event that isn't monitored just so we have a valid EventTypeSpec to install
		{ kEventClassCommand, kEventCommandUpdateStatus },
		{ kEventClassKeyboard, kEventRawKeyDown },
		{ kEventClassKeyboard, kEventRawKeyUp },
		{ kEventClassKeyboard, kEventRawKeyRepeat },
		{ kEventClassKeyboard, kEventRawKeyModifiersChanged },
		{ kEventClassMouse, kEventMouseDown },
		{ kEventClassMouse, kEventMouseUp },
		{ kEventClassMouse, kEventMouseMoved },
		{ kEventClassMouse, kEventMouseDragged },
		{ kEventClassMouse, kEventMouseWheelMoved }
		/*
		{ kEventClassTablet, kEventTabletPoint },
		{ kEventClassTablet, kEventTabletProximity }
		*/
	};
		
	EventTypeSpec	kCmdEvents[] = {
		{ kEventClassCommand, kEventCommandProcess }
	};

	EventTypeSpec kSuspendResumeEvents[] = {
		{ kEventClassApplication,kEventAppActivated },
		{ kEventClassApplication,kEventAppDeactivated }
	};

	// install an event handler to detect clicks on the main window to tell the application which 
	// events to track/stop tracking
	//InstallApplicationEventHandler( CmdHandler, GetEventTypeCount( kCmdEvents ), kCmdEvents, 0, &fHandler );
	
	// the magic happens here with the call to GetEventMonitorTarget as described in CarbonEvents.h
	// The event monitor target is a special event target used to monitor user input events across all processes.
	// When the monitor target detects a matching event, then MonitorHandler is called.
	InstallEventHandler( GetEventMonitorTarget(), MonitorHandler, GetEventTypeCount( kEvents ), kEvents, 0, &sHandler );
	
	// handle suspend and resume events to detect when these transitions occur so that we know when this test 
	// process is in the foreground or not
	//InstallApplicationEventHandler(NewEventHandlerUPP(SuspendResumeHandler), GetEventTypeCount( kSuspendResumeEvents ), kSuspendResumeEvents, 0, NULL);
	return noErr;
}


int main(int argc, char* argv[]) {
	SInt16 rep = LMGetKeyRepThresh();
	SInt16 del = LMGetKeyThresh ();
	fprintf(stderr, "rep %d\ndel %d\n", rep, del);

	char namebuf[256];
	CFTypeRef value = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (value != NULL && CFGetTypeID(value) == CFNumberGetTypeID()) {
		int intval = 0;
		CFNumberGetValue((CFNumberRef)value, kCFNumberSInt32Type, &intval);
		fprintf(stderr, "%i", intval);
	}

	// install the various event handlers.
	InstallMyEventHandlers();

	// Call the event loop
	RunApplicationEventLoop();
}
