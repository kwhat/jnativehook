/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

//TODO Check to see if there is anything useful in here...
char * KeyCodeToString(Display * display, KeyCode keycode, unsigned int event_mask) {
	static UInt32 deadKeyState = 0, keyTranslateState = 0;
	static SInt32 lastKeyLayoutID = 0;

	UCKeyboardLayout *uchrData;
	void  *KCHRData;
	SInt32  keyLayoutKind, keyLayoutID;
	KeyboardLayoutRef keyLayout;
	UInt32  charCode;
	UniCharCount  actualStringLength;
	UniChar  unicodeString[4];
	unsigned char code1, code2;

	UInt16 keyCode = [ev keyCode], kchrKeyCode, uniChar = 0;
	unsigned int mods = [ev modifierFlags];

	KLGetCurrentKeyboardLayout(&keyLayout);
	KLGetKeyboardLayoutProperty(keyLayout, kKLKind, (const void **) &keyLayoutKind);
	KLGetKeyboardLayoutProperty(keyLayout, kKLIdentifier, (const void**) &keyLayoutID);

	if (lastKeyLayoutID != keyLayoutID) {
	NSLog(@"New key layout. Old: %d, new: %d\n", lastKeyLayoutID, keyLayoutID);
	deadKeyState = keyTranslateState = 0;
	lastKeyLayoutID = keyLayoutID;
	}

	if (keyLayoutKind == kKLKCHRKind) {
	NSLog(@"Handling a kchr key event");
	
	//this might have a slight logic error in it. be careful, test it first
	kchrKeyCode = (keyCode & 0x7f) | (mods & 0xffff00ff);
	
	//Use a preprocessor next time??
	if (YES /* NO if handling a key up */) {
		kchrKeyCode =& 0xff7f
	}
	else {
		kchrKeyCode =| 0x80;
	}

	KLGetKeyboardLayoutProperty(keyLayout, kKLKCHRData, (const void **)&KCHRData);
	charCode = KeyTranslate(KCHRData, kchrKeyCode, &keyTranslateState);

	// ignore code1, it's normally gibberish
	//code1 = (charCode & 0xff0000) >> 16;
	code2 = (charCode & 0xff);

	if (keyTranslateState == 0 && code2 != '\0') {
		const char chrs[2] = {code2, '\0'};

		//I don't remember why this is used, there's probably a reason
		uniChar = [[NSString stringWithCString:chrs]characterAtIndex:0]; 
	}
	}
	else {
	NSLog(@"Handling a uchr key event");
	KLGetKeyboardLayoutProperty(keyLayout, kKLuchrData, (const void **) &uchrData);
	UCKeyTranslate(uchrData, keyCode, kUCKeyActionDown, mods, LMGetKbdType(), 0, &deadKeyState, 4, &actualStringLength, unicodeString);
	uniChar = unicodeString[0];
	}

	NSLog(@"'%C' (virtual key 0x%x, unicode 0x%x) pressed", uniChar, keyCode, uniChar);
}

