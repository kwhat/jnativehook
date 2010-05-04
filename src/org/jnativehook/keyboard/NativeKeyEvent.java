/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.keyboard;

//Imports
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeInputEvent;
 
/**
 * An event which indicates that a keystroke occurred on the system.
 * <p>
 * TODO Add more in depth description.  
 */
public class NativeKeyEvent extends NativeInputEvent {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 5228137904514960737L;
	
	/** The raw native key code. */
	private int rawCode;
	
	/** The virtual key code. */
	private int keyCode;
	
	/** The Unicode key char. */
	private char keyChar;
	
	/** The virtual key location. */
	private int keyLocation;
	
	
	public static final char CHAR_UNDEFINED = KeyEvent.CHAR_UNDEFINED; 
	
	/** The first number in the range of id's used for native key events. */
	public static final int NATIVE_KEY_FIRST = 2400;
	
	/** The last number in the range of id's used for native key events. */
	public static final int NATIVE_KEY_LAST = 2402;
	
	/** The "native key typed" event id. */
	public static final int NATIVE_KEY_TYPED = NATIVE_KEY_FIRST;
	
	/** The "native key pressed" event id. */
	public static final int NATIVE_KEY_PRESSED = 1 + NATIVE_KEY_FIRST;
	
	/** The "native key released" event id. */
	public static final int NATIVE_KEY_RELEASED = 2 + NATIVE_KEY_FIRST;

	/** VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	public static final int VK_0 = 0x30;
	public static final int VK_1 = 0x31;
	public static final int VK_2 = 0x32;
	public static final int VK_3 = 0x33;
	public static final int VK_4 = 0x34;
	public static final int VK_5 = 0x35;
	public static final int VK_6 = 0x36;
	public static final int VK_7 = 0x37;
	public static final int VK_8 = 0x39;
	public static final int VK_9 = 0x39;
	
	/** VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
	public static final int VK_A = 0x41;
	public static final int VK_B = 0x42;
	public static final int VK_C = 0x43;
	public static final int VK_D = 0x44;
	public static final int VK_E = 0x45;
	public static final int VK_F = 0x46;
	public static final int VK_G = 0x47;
	public static final int VK_H = 0x48;
	public static final int VK_I = 0x49;
	public static final int VK_J = 0x4A;
	public static final int VK_K = 0x4B;
	public static final int VK_L = 0x4C;
	public static final int VK_M = 0x4D;
	public static final int VK_N = 0x4E;
	public static final int VK_O = 0x4F;
	public static final int VK_P = 0x50;
	public static final int VK_Q = 0x51;
	public static final int VK_R = 0x52;
	public static final int VK_S = 0x53;
	public static final int VK_T = 0x54;
	public static final int VK_U = 0x55;
	public static final int VK_V = 0x56;
	public static final int VK_W = 0x57;
	public static final int VK_X = 0x58;
	public static final int VK_Y = 0x59;
	public static final int VK_Z = 0x5A;
	
	public static final int VK_SHIFT = KeyEvent.VK_SHIFT;
	public static final int VK_CONTROL = KeyEvent.VK_CONTROL;
	public static final int VK_META = KeyEvent.VK_META;
	public static final int VK_CONTEXT_MENU = KeyEvent.VK_CONTEXT_MENU;
	public static final int VK_ALT = KeyEvent.VK_ALT;
	
	public static final int VK_TAB = KeyEvent.VK_TAB;
	public static final int VK_CAPS_LOCK = KeyEvent.VK_CAPS_LOCK;
	
	public static final int VK_SPACE = KeyEvent.VK_SPACE;
	
	public static final int VK_BACK_QUOTE = KeyEvent.VK_BACK_QUOTE;
	public static final int VK_MINUS = KeyEvent.VK_MINUS;
	public static final int VK_EQUALS = KeyEvent.VK_EQUALS;
	
	public static final int VK_OPEN_BRACKET = KeyEvent.VK_OPEN_BRACKET;
	public static final int VK_CLOSE_BRACKET = KeyEvent.VK_CLOSE_BRACKET;
	public static final int VK_BACK_SLASH = KeyEvent.VK_BACK_SLASH;
	
	public static final int VK_SEMICOLON = KeyEvent.VK_SEMICOLON;
	public static final int VK_QUOTE = KeyEvent.VK_QUOTE;
	public static final int VK_ENTER = KeyEvent.VK_ENTER;
	
	public static final int VK_COMMA = KeyEvent.VK_COMMA;
	public static final int VK_PERIOD = KeyEvent.VK_PERIOD;
	public static final int VK_SLASH = KeyEvent.VK_SLASH;
	
	public static final int VK_ESCAPE = KeyEvent.VK_ESCAPE;
	public static final int VK_BACK_SPACE = KeyEvent.VK_BACK_SPACE;
	
	/** Constants for the F1 thru F24 function keys. */
	public static final int VK_F1 = KeyEvent.VK_F1;
	public static final int VK_F2 = KeyEvent.VK_F2;
	public static final int VK_F3 = KeyEvent.VK_F3;
	public static final int VK_F4 = KeyEvent.VK_F4;
	public static final int VK_F5 = KeyEvent.VK_F5;
	public static final int VK_F6 = KeyEvent.VK_F6;
	public static final int VK_F7 = KeyEvent.VK_F7;
	public static final int VK_F8 = KeyEvent.VK_F8;
	public static final int VK_F9 = KeyEvent.VK_F9;
	public static final int VK_F10 = KeyEvent.VK_F10;
	public static final int VK_F11 = KeyEvent.VK_F11;
	public static final int VK_F12 = KeyEvent.VK_F12;
	public static final int VK_F13 = KeyEvent.VK_F13;
	public static final int VK_F14 = KeyEvent.VK_F14;
	public static final int VK_F15 = KeyEvent.VK_F15;
	public static final int VK_F16 = KeyEvent.VK_F16;
	public static final int VK_F17 = KeyEvent.VK_F17;
	public static final int VK_F18 = KeyEvent.VK_F18;
	public static final int VK_F19 = KeyEvent.VK_F19;
	public static final int VK_F20 = KeyEvent.VK_F20;
	public static final int VK_F21 = KeyEvent.VK_F21;
	public static final int VK_F22 = KeyEvent.VK_F22;
	public static final int VK_F23 = KeyEvent.VK_F23;
	public static final int VK_F24 = KeyEvent.VK_F24;
	
	public static final int VK_PRINTSCREEN = KeyEvent.VK_PRINTSCREEN;
	public static final int VK_SCROLL_LOCK = KeyEvent.VK_SCROLL_LOCK;
	public static final int VK_PAUSE = KeyEvent.VK_PAUSE;
	
	public static final int VK_INSERT = KeyEvent.VK_INSERT;
	public static final int VK_DELETE = KeyEvent.VK_DELETE;
	public static final int VK_HOME = KeyEvent.VK_HOME;
	public static final int VK_END = KeyEvent.VK_END;
	public static final int VK_PAGE_UP = KeyEvent.VK_PAGE_UP;
	public static final int VK_PAGE_DOWN = KeyEvent.VK_PAGE_DOWN;
	
	public static final int VK_BEGIN = KeyEvent.VK_BEGIN;
	
	public static final int VK_UP = KeyEvent.VK_UP;
	public static final int VK_DOWN = KeyEvent.VK_DOWN;
	public static final int VK_LEFT = KeyEvent.VK_LEFT;
	public static final int VK_RIGHT = KeyEvent.VK_RIGHT;
	
	
	public static final int VK_NUMPAD0 = KeyEvent.VK_NUMPAD0;
	public static final int VK_NUMPAD1 = KeyEvent.VK_NUMPAD1;
	public static final int VK_NUMPAD2 = KeyEvent.VK_NUMPAD2;
	public static final int VK_NUMPAD3 = KeyEvent.VK_NUMPAD3;
	public static final int VK_NUMPAD4 = KeyEvent.VK_NUMPAD4;
	public static final int VK_NUMPAD5 = KeyEvent.VK_NUMPAD5;
	public static final int VK_NUMPAD6 = KeyEvent.VK_NUMPAD6;
	public static final int VK_NUMPAD7 = KeyEvent.VK_NUMPAD7;
	public static final int VK_NUMPAD8 = KeyEvent.VK_NUMPAD8;
	public static final int VK_NUMPAD9 = KeyEvent.VK_NUMPAD9;
	
	public static final int VK_KP_UP = KeyEvent.VK_KP_UP;
	public static final int VK_KP_DOWN = KeyEvent.VK_KP_DOWN;
	public static final int VK_KP_LEFT = KeyEvent.VK_KP_LEFT;
	public static final int VK_KP_RIGHT = KeyEvent.VK_KP_RIGHT;
	
	public static final int VK_NUM_LOCK = KeyEvent.VK_NUM_LOCK;
	public static final int VK_DIVIDE = KeyEvent.VK_DIVIDE;
	public static final int VK_MULTIPLY = KeyEvent.VK_MULTIPLY;
	public static final int VK_SUBTRACT = KeyEvent.VK_SUBTRACT;
	public static final int VK_PLUS = KeyEvent.VK_PLUS;
	public static final int VK_DECIMAL = KeyEvent.VK_DECIMAL;
	
	
	/**
	 * Instantiates a new native key event.
	 * <p>
	 * Note that passing in an invalid id results in unspecified behavior.
	 * @param id - the type of event
	 * @param when - the time the event occurred
	 * @param modifiers - the modifier keys down during event (shift, ctrl, alt, meta).
	 * FIXME Either extended _DOWN_MASK or old _MASK modifiers should be used, but both models should not be mixed in one event. Use of the extended modifiers is preferred.
	 * @param rawCode - The native system key for this event.  This is the number used to represent a symbols visible on a keyboard  and not the translated key code it may represent.  
	 * @param keyCode - The virtual key code generated by this event
	 * @param keyChar - The Unicode char generated by this event
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode, char keyChar) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
		
		if (id == NativeKeyEvent.NATIVE_KEY_TYPED) {
			if (keyChar == KeyEvent.CHAR_UNDEFINED) {
				throw new IllegalArgumentException("invalid keyChar");
			}
			else if (keyCode != KeyEvent.VK_UNDEFINED) {
				throw new IllegalArgumentException("invalid keyCode");
			}
			else if (keyLocation != KeyEvent.KEY_LOCATION_UNKNOWN) {
				throw new IllegalArgumentException("invalid keyLocation");
			}
		}
		
		this.rawCode = rawCode;
		this.keyCode = keyCode;
		this.keyChar = keyChar;
	}
	
	/**
	 * Instantiates a new native key event.
	 * <p>
	 * Note that passing in an invalid id results in unspecified behavior.
	 * @param id - the type of event
	 * @param when - the time the event occurred
	 * @param modifiers - the modifier keys down during event (shift, ctrl, alt, meta).
	 * FIXME Either extended _DOWN_MASK or old _MASK modifiers should be used, but both models should not be mixed in one event. Use of the extended modifiers is preferred.
	 * @param rawCode - The native system key for this event.  This is the number used to represent a symbols visible on a keyboard  and not the translated key code it may represent.  
	 * @param keyCode - The virtual key code generated by this event
	 * @param keyChar - The Unicode char generated by this event
	 * @param keyLocation - the location id of the key generating this event.
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode, char keyChar, int keyLocation) {
		this(id, when, modifiers, rawCode, keyCode, keyChar);
		
		this.keyLocation = keyLocation;
	}

	
	/**
	 * Returns the rawCode associated with the native key in this event.
	 *
	 * @return the native system key for this event.  This is the number used to represent a symbols visible on a keyboard  and not the translated key code it may represent.
	 */
	public int getRawCode() {
		return this.rawCode;
	}
	
	/**
	 * Set the rawCode value in this event.
	 *
	 * @param rawCode - The native system key for this event.  This is the number used to represent a symbols visible on a keyboard  and not the translated key code it may represent.
	 */
	public void setRawCode(int rawCode) {
		 this.rawCode = rawCode;
	}
	
	
	/**
	 * Returns the character associated with the virtual key in this event.
	 * FIXME Check and make sure the key-typed event for shift + "a" returns the value for "A".
	 * @return the Unicode character defined for this key event.
	 */
	public char getKeyChar() {
		return this.keyChar;
	}
	
	/**
	 * Set the keyChar value in this event.
	 * 
	 * @param keyChar - The Unicode char generated by this event
	 */
	public void setKeyChar(char keyChar) {
		 this.keyChar = keyChar;
	}

	/**
	 * Returns the keyCode associated with the virtual key in this event.
	 * FIXME Always return VK_UNDEFINED for NATIVE_KEY_TYPED events.
	 * @return the virtual key code generated by this event. Always returns VK_UNDEFINED for NATIVE_KEY_TYPED events.
	 */
	public int getKeyCode() {
		return this.keyCode;
	}
	
	/**
	 * Set the keyCode value in this event.
	 *
	 * @param keyCode - The virtual key code generated by this event
	 */
	public void setKeyCode(int keyCode) {
		 this.keyCode = keyCode;
	}
	
	/**
	 * Returns the location of the virtual key for this event.
	 * FIXME Always return KEY_LOCATION_UNKNOWN for NATIVE_KEY_TYPED events.
	 * @return the location of the virtual key that was pressed or released. Always returns KEY_LOCATION_UNKNOWN for NATIVE_KEY_TYPED events.
	 */
	public int getKeyLocation() {
		return this.keyLocation;
	}
	
	/**
	 * Returns a String describing the keyCode, such as "HOME", "F1" or "A". These strings can be localized by changing the awt.properties file.
	 *
	 * @param keyCode - The virtual key code generated by this event
	 * @return a string containing a text description for a physical key, identified by its keyCode
	 */
	public static String getKeyText(int keyCode) {
		return KeyEvent.getKeyText(keyCode);
	}
	
	/**
	 * Gets the key modifiers text.
	 *
	 * @param modifiers - the modifier keys down during event (shift, ctrl, alt, meta).
	 * @return the key modifiers text
	 */
	public static String getKeyModifiersText(int modifiers) {
		return KeyEvent.getKeyModifiersText(modifiers);
	}
	

	
	/**
	 * Returns a parameter string identifying this event. This method is useful for event logging and for debugging.
	 * 
	 * @return a string identifying the event and its attributes
	 */
	public String paramString() {
		String param = "";
		
		switch(getId()) {
			case NATIVE_KEY_TYPED:
				param += "NATIVE_KEY_TYPED";
			break;
			
			case NATIVE_KEY_PRESSED:
				param += "NATIVE_KEY_PRESSED";
			break;
			
			case NATIVE_KEY_RELEASED:
				param += "NATIVE_KEY_RELEASED";
			break;
			
			default:
				param += "unknown type";
			break;
		}
		param += ",";
		
		param += "keyCode=" + keyCode + ",";
		param += "keyText=" + getKeyText(keyCode) + ",";
		
		param += "keyChar=";
		switch (keyChar) {
			case KeyEvent.CHAR_UNDEFINED:
				param += Toolkit.getProperty("AWT.undefined", "Undefined") + " keyChar";
			break;
			
			case KeyEvent.VK_BACK_SPACE:
			case KeyEvent.VK_TAB:
			case KeyEvent.VK_ENTER:
			case KeyEvent.VK_CANCEL:
			case KeyEvent.VK_ESCAPE:
			case KeyEvent.VK_DELETE:
			case KeyEvent.VK_SHIFT:
			case KeyEvent.VK_ALT:
			case KeyEvent.VK_CONTROL:
			case KeyEvent.VK_CONTEXT_MENU:
				param += getKeyText(keyChar);
			break;
			
			default:
				param += "'" + keyChar + "'";
			break;
		}
		param += ",";
		
		
		if (getModifiers() != 0) {
			param += "modifiers=" + getKeyModifiersText(getModifiers()) + ",";
		}
		
		if (getModifiersEx() != 0) {
			param += "extModifiers=" + getModifiersExText(getModifiersEx()) + ",";
		}

		param += "keyLocation=";
		switch (keyLocation) {
			case KeyEvent.KEY_LOCATION_UNKNOWN:
				param += "KEY_LOCATION_UNKNOWN";
			break;
			
			case KeyEvent.KEY_LOCATION_STANDARD:
				param += "KEY_LOCATION_STANDARD";
			break;
			
			case KeyEvent.KEY_LOCATION_LEFT:
				param += "KEY_LOCATION_LEFT";
			break;
			
			case KeyEvent.KEY_LOCATION_RIGHT:
				param += "KEY_LOCATION_RIGHT";
			break;
			
			case KeyEvent.KEY_LOCATION_NUMPAD:
				param += "KEY_LOCATION_NUMPAD";
			break;
			
			default:
				param += "KEY_LOCATION_UNKNOWN";;
			break;
		}
		param += ",";
		
		param += "rawCode=" + rawCode;
		
		return param;
	}
}
