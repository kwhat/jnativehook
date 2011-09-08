/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Carbon/Carbon.h>

CFRunLoopRef event_loop;

CGEventRef eventHandlerCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * refcon) {
	//Event Data
	CGPoint point;
	UInt64	vk_code, button;
	CGEventFlags event_mask = CGEventGetFlags(event);

	// get the event class
	switch (CGEventGetType(event)) {
		case kCGEventKeyDown:
			vk_code = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			printf("Key Press - %i\n", (int) vk_code);

			//Stop looping if the escape key is pressed.
			if (vk_code == kVK_Escape) {
				CFRunLoopStop(event_loop);
			}
		break;

		case kCGEventKeyUp:
			vk_code = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			printf("Key Release - %i\n", (int) vk_code);
		break;

		case kCGEventFlagsChanged:
			printf("Modifiers Changed - %x\n", (unsigned int) event_mask);
		break;

		case kCGEventLeftMouseDown:
		case kCGEventRightMouseDown:
		case kCGEventOtherMouseDown:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);
			printf("Button Press - %i\n", (int) button);
		break;

		case kCGEventLeftMouseUp:
		case kCGEventRightMouseUp:
		case kCGEventOtherMouseUp:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);
			printf("Button Release - %i\n", (int) button);
		break;


		case kCGEventLeftMouseDragged:
		case kCGEventRightMouseDragged:
		case kCGEventOtherMouseDragged:
			//Call mouse move events for now.  Adding this functionality to other systems will be difficult.

		case kCGEventMouseMoved:
			point = CGEventGetLocation(event);
			printf("Motion Notify - %f, %f\n", point.x, point.y);
		break;

		case kCGEventScrollWheel:
			//Do Nothing.
		break;
	}

	return noErr;
}

int main(int argc, char* argv[]) {
	//Check and make sure assistive devices is enabled.
	if (! AXAPIEnabled()) {
		printf("Error: Please enabled access for assistive devices in the Universal Access section of the System Preferences.\n");
		return 1;
	}

	CGEventMask event_mask =	CGEventMaskBit(kCGEventKeyDown) |
								CGEventMaskBit(kCGEventKeyUp) |
								CGEventMaskBit(kCGEventFlagsChanged) |

								CGEventMaskBit(kCGEventLeftMouseDown) |
								CGEventMaskBit(kCGEventLeftMouseUp) |
								CGEventMaskBit(kCGEventLeftMouseDragged) |

								CGEventMaskBit(kCGEventRightMouseDown) |
								CGEventMaskBit(kCGEventRightMouseUp) |
								CGEventMaskBit(kCGEventRightMouseDragged) |

								CGEventMaskBit(kCGEventOtherMouseDown) |
								CGEventMaskBit(kCGEventOtherMouseUp) |
								CGEventMaskBit(kCGEventOtherMouseDragged) |

								CGEventMaskBit(kCGEventMouseMoved) |
								CGEventMaskBit(kCGEventScrollWheel);


    CFMachPortRef event_port = CGEventTapCreate(
									kCGSessionEventTap,
									kCGHeadInsertEventTap,
									kCGEventTapOptionListenOnly,
									event_mask,
									eventHandlerCallback,
									NULL
                                );

	if (event_port == NULL) {
		printf("Error: Could not create event tap.\n");
		return 1;
	}

	CFRunLoopSourceRef event_source = CFMachPortCreateRunLoopSource(NULL, event_port, 0);
	if (event_source == NULL) {
		printf("Error: Could not create run loop source.\n");
		return 1;
	}

	event_loop = CFRunLoopGetCurrent();
	if (event_loop == NULL) {
		printf("Error: Could not attach to the current run loop.\n");
		return 1;
	}

	CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
	CFRunLoopRun();
	CFRunLoopRemoveSource(event_loop, event_source, kCFRunLoopDefaultMode);

	return 0;
}
