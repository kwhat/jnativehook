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

EventHandlerRef handler;

OSStatus eventHandlerCallback(EventHandlerCallRef caller, EventRef event, void * refcon) {
	Point	point;
	UInt32	vk_code;
	EventMouseButton button;

	// get the event class
	if (GetEventClass(event) == kEventClassKeyboard) {
			//Get the keyboard event type
			switch (GetEventKind(event)) {
				case kEventRawKeyDown:
					GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &vk_code);
					printf("Key Press - %i\n", (int) vk_code);

					//Stop looping if the escape key is pressed.
					if (vk_code == kVK_Escape) {
						RemoveEventHandler(handler);
						exit(0);
					}
				break;

				case kEventRawKeyUp:
					GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &vk_code);
					printf("Key Release - %i\n", (int) vk_code);
				break;

				case kEventRawKeyRepeat:
					GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &vk_code);
					printf("Key Repeat - %i\n", (int) vk_code);
				break;

				case kEventRawKeyModifiersChanged:
					GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &vk_code);
					printf("Modifiers Changed - %x\n", (unsigned int) vk_code);
				break;
			}
	}
	else if (GetEventClass(event) == kEventClassMouse) {
		//Get the type or mouse event
		switch (GetEventKind(event)) {
			case kEventMouseDown:
				GetEventParameter(event, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
				printf("Button Press - %i\n", (int) button);
			break;

			case kEventMouseUp:
				GetEventParameter(event, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
				printf("Button Release - %i\n", (int) button);
			break;

			case kEventMouseMoved:
				GetEventParameter(event, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &point);
				printf("Motion Notify - %i, %i\n", point.v, point.h);
			break;

			case kEventMouseDragged:
				//Do Nothing.
			break;

			case kEventMouseWheelMoved:
				//Do Nothing.
			break;
		}
	}
	
	return noErr;
}

int main(int argc, char* argv[]) {
	//Check and make sure assistive devices is enabled.
	if (! AXAPIEnabled()) {
		printf("Please enabled access for assistive devices in the Universal Access section of the System Preferences.\n");
		return 1;
	}

	EventTypeSpec kEvents[] = {
		//Use an event that isn't monitored just so we have a valid EventTypeSpec to install
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

	//Install the event handler.
	InstallEventHandler(GetEventMonitorTarget(), eventHandlerCallback, GetEventTypeCount(kEvents), kEvents, 0, &handler);

	//Call the event loop.
	RunApplicationEventLoop();

	return 0;
}
