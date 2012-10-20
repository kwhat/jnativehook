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

#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	CGEventRef event_down = CGEventCreateKeyboardEvent(NULL, (CGKeyCode) 11 /* B Key*/, true);
	CGEventSetFlags(event_down, (CGEventFlags) 0x00);
	
	CGEventRef event_up = CGEventCreateKeyboardEvent(NULL, (CGKeyCode) 11 /* B Key*/, false);
	CGEventSetFlags(event_up, (CGEventFlags) 0x00);

		int i = 0;
	for (i = 0; i < 5; i++) {
		sleep(1) ;
		
		CGEventPost(kCGSessionEventTap, event_down);
		
		usleep(20000);
		
		CGEventPost(kCGSessionEventTap, event_up);
	}
	
	return 0;
}
