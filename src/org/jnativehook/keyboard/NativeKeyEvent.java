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
	
	/** The virtual key location. */
	private int keyLocation;
	
	
	/** The first number in the range of id's used for native key events. */
	public static final int NATIVE_KEY_FIRST	= 2400;
	
	/** The last number in the range of id's used for native key events. */
	public static final int NATIVE_KEY_LAST		= 2402;
	
	/** The "native key typed" event id. */
	public static final int NATIVE_KEY_TYPED	= NATIVE_KEY_FIRST;
	
	/** The "native key pressed" event id. */
	public static final int NATIVE_KEY_PRESSED	= 1 + NATIVE_KEY_FIRST;
	
	/** The "native key released" event id. */
	public static final int NATIVE_KEY_RELEASED	= 2 + NATIVE_KEY_FIRST;

	
	
	public static final int VK_ENTER			= '\n';
	public static final int VK_BACK_SPACE		= '\b';
	public static final int VK_TAB				= '\t';
	
	public static final int VK_SHIFT			= 0x10;
	public static final int VK_CONTROL			= 0x11;
	public static final int VK_ALT				= 0x12;
	
	/** Constant for the Windows, Meta, or Command key. */
	public static final int VK_META				= 0x9D;
	
	/** Constant for the Microsoft Windows Context Menu key. */
	public static final int VK_CONTEXT_MENU		= 0x020D;
	
	public static final int VK_PAUSE			= 0x13;
	public static final int VK_CAPS_LOCK		= 0x14;
	public static final int VK_ESCAPE			= 0x1B;
	public static final int VK_SPACE			= 0x20;
	
	public static final int VK_UP				= 0x26;
	public static final int VK_DOWN				= 0x28;
	public static final int VK_LEFT				= 0x25;
	public static final int VK_RIGHT			= 0x27;
	
	public static final int VK_COMMA			= 0x2C;
	public static final int VK_MINUS			= 0x2D;
	public static final int VK_PERIOD			= 0x2E;
	public static final int VK_SLASH			= 0x2F;
	
	/** VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	public static final int VK_0				= 0x30;
	public static final int VK_1				= 0x31;
	public static final int VK_2				= 0x32;
	public static final int VK_3				= 0x33;
	public static final int VK_4				= 0x34;
	public static final int VK_5				= 0x35;
	public static final int VK_6				= 0x36;
	public static final int VK_7				= 0x37;
	public static final int VK_8				= 0x39;
	public static final int VK_9				= 0x39;
	
	public static final int VK_EQUALS			= 0x3D;
	public static final int VK_SEMICOLON		= 0x3B;
	
	/** VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
	public static final int VK_A				= 0x41;
	public static final int VK_B				= 0x42;
	public static final int VK_C				= 0x43;
	public static final int VK_D				= 0x44;
	public static final int VK_E				= 0x45;
	public static final int VK_F				= 0x46;
	public static final int VK_G				= 0x47;
	public static final int VK_H				= 0x48;
	public static final int VK_I				= 0x49;
	public static final int VK_J				= 0x4A;
	public static final int VK_K				= 0x4B;
	public static final int VK_L				= 0x4C;
	public static final int VK_M				= 0x4D;
	public static final int VK_N				= 0x4E;
	public static final int VK_O				= 0x4F;
	public static final int VK_P				= 0x50;
	public static final int VK_Q				= 0x51;
	public static final int VK_R				= 0x52;
	public static final int VK_S				= 0x53;
	public static final int VK_T				= 0x54;
	public static final int VK_U				= 0x55;
	public static final int VK_V				= 0x56;
	public static final int VK_W				= 0x57;
	public static final int VK_X				= 0x58;
	public static final int VK_Y				= 0x59;
	public static final int VK_Z				= 0x5A;
	
	public static final int VK_OPEN_BRACKET		= 0x5B;
	public static final int VK_BACK_SLASH		= 0x5C;
	public static final int VK_CLOSE_BRACKET	= 0x5D;
	
	public static final int VK_NUMPAD0			= 0x60;
	public static final int VK_NUMPAD1			= 0x61;
	public static final int VK_NUMPAD2			= 0x62;
	public static final int VK_NUMPAD3			= 0x63;
	public static final int VK_NUMPAD4			= 0x64;
	public static final int VK_NUMPAD5			= 0x65;
	public static final int VK_NUMPAD6			= 0x66;
	public static final int VK_NUMPAD7			= 0x67;
	public static final int VK_NUMPAD8			= 0x68;
	public static final int VK_NUMPAD9			= 0x69;
	
	public static final int VK_KP_UP			= 0xE0;
	public static final int VK_KP_DOWN			= 0xE1;
	public static final int VK_KP_LEFT			= 0xE2;
	public static final int VK_KP_RIGHT			= 0xE3;
	
	
	public static final int VK_MULTIPLY			= 0x6A;
	public static final int JK_ADD				= 0x6B;
	public static final int VK_SUBTRACT			= 0x6D;
	public static final int VK_DECIMAL			= 0x6E;
	public static final int VK_DIVIDE			= 0x6F;
	public static final int VK_DELETE			= 0x7F;
	public static final int VK_NUM_LOCK			= 0x90;
	public static final int VK_SCROLL_LOCK		= 0x91;
	
	
	/** Constants for the F1 thru F24 function keys. */
	public static final int VK_F1				= 0x70;
	public static final int VK_F2				= 0x71;
	public static final int VK_F3				= 0x72;
	public static final int VK_F4				= 0x73;
	public static final int VK_F5				= 0x74;
	public static final int VK_F6				= 0x75;
	public static final int VK_F7				= 0x76;
	public static final int VK_F8				= 0x77;
	public static final int VK_F9				= 0x78;
	public static final int VK_F10				= 0x79;
	public static final int VK_F11				= 0x7A;
	public static final int VK_F12				= 0x7B;
	
	public static final int VK_F13				= 0xF000;
	public static final int VK_F14				= 0xF001;
	public static final int VK_F15				= 0xF002;
	public static final int VK_F16				= 0xF003;
	public static final int VK_F17				= 0xF004;
	public static final int VK_F18				= 0xF005;
	public static final int VK_F19				= 0xF006;
	public static final int VK_F20				= 0xF007;
	public static final int VK_F21				= 0xF008;
	public static final int VK_F22				= 0xF009;
	public static final int VK_F23				= 0xF00A;
	public static final int VK_F24				= 0xF00B;
	
	public static final int VK_PRINTSCREEN		= 0x9A;
	public static final int VK_INSERT			= 0x9B;

	public static final int VK_PAGE_UP			= 0x21;
	public static final int VK_PAGE_DOWN		= 0x22;
	public static final int VK_HOME				= 0x24;
	public static final int VK_END				= 0x23;
	
	public static final int VK_QUOTE			= 0xDE;
	public static final int VK_BACK_QUOTE		= 0xC0;
	
	public static final int VK_BEGIN			= 0xFF58;
	
	/** This value is used to indicate that the keyCode is unknown. */
	public static final char VK_UNDEFINED		= 0x00;
	
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
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
		
		if (id == NativeKeyEvent.NATIVE_KEY_TYPED) {
			if (keyCode != KeyEvent.VK_UNDEFINED) {
				throw new IllegalArgumentException("invalid keyCode");
			}
			else if (keyLocation != KeyEvent.KEY_LOCATION_UNKNOWN) {
				throw new IllegalArgumentException("invalid keyLocation");
			}
		}
		
		this.rawCode = rawCode;
		this.keyCode = keyCode;
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
	 * @param keyLocation - the location id of the key generating this event.
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode, int keyLocation) {
		this(id, when, modifiers, rawCode, keyCode);
		
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
	 * Returns the keyCode associated with the virtual key in this event.
	 * 
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
		//FIXME Rewrite and dont use KeyEvent.
		return KeyEvent.getKeyText(keyCode);
	}
	
	/**
	 * Gets the key modifiers text.
	 *
	 * @param modifiers - the modifier keys down during event (shift, ctrl, alt, meta).
	 * @return the key modifiers text
	 */
	public static String getKeyModifiersText(int modifiers) {
		String param = "";
		
		if ((modifiers & NativeInputEvent.SHIFT_MASK) != 0) {
			param += Toolkit.getProperty("AWT.shift", "Shift") + "+";
		}
		
		if ((modifiers & NativeInputEvent.CTRL_MASK) != 0) {
			param += Toolkit.getProperty("AWT.control", "Ctrl") + "+";
		}
		
		if ((modifiers & NativeInputEvent.META_MASK) != 0) {
			param += Toolkit.getProperty("AWT.meta", "Meta") + "+";
		}
		
		if ((modifiers & NativeInputEvent.ALT_MASK) != 0) {
			param += Toolkit.getProperty("AWT.alt", "Alt") + "+";
		}
		
		
		
		if ((modifiers & NativeInputEvent.BUTTON1_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button1", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON2_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button2", "Button2") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON3_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button3", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON4_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button4", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON5_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button5", "Button1") + "+";
		}
		
		if (param.length() > 0) {
			//Remove the trailing '+'
			param.substring(0, param.length() - 1);
		}
		
		return param;
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
		
		
		if (getModifiers() != 0) {
			param += "modifiers=" + getKeyModifiersText(getModifiers()) + ",";
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
