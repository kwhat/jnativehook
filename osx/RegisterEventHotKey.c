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

OSStatus handler(EventHandlerCallRef nextHandler, EventRef event, void* userData) {
	printf("Global keyboard shortcut pressed!\n");
	return noErr;
}

int main() {
	EventTypeSpec eventType;
	eventType.eventClass = kEventClassKeyboard;
	eventType.eventKind  = kEventHotKeyPressed;

	//Install an event handler for keyboard hotkey pressed.
	InstallApplicationEventHandler(&handler, 1, &eventType, NULL, NULL);

	EventHotKeyID eventHotKey;
	eventHotKey.id = 1;

	EventHotKeyRef eventHotKeyRef;

	//http://boredzo.org/blog/wp-content/uploads/2007/05/IMTx-virtual-keycodes.pdf
	//Keycode 11 is the B key.
	RegisterEventHotKey(11, controlKey + cmdKey, eventHotKey, GetApplicationEventTarget(), 0, &eventHotKeyRef);

	printf("Press control + command + b...\n");

	//Wait for events.
	RunApplicationEventLoop();

	return 0;
}
