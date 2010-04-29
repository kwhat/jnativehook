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
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeKeyEvent.
 */
public class NativeKeyEvent extends NativeInputEvent {
	private static final long serialVersionUID = 5228137904514960737L;
	private int rawCode;
	private int keyCode;
	private char keyChar;
	private int keyLocation;
	
	public static final int NATIVE_KEY_FIRST = 2400;
	public static final int NATIVE_KEY_LAST = 2402;
	public static final int NATIVE_KEY_TYPED = NATIVE_KEY_FIRST;
	public static final int NATIVE_KEY_PRESSED = 1 + NATIVE_KEY_FIRST;
	public static final int NATIVE_KEY_RELEASED = 2 + NATIVE_KEY_FIRST;

	
	/**
	 * Instantiates a new native key event.
	 *
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iKeyCode the i key code
	 * @param cKeyChar the c key char
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode, char keyChar) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
		
		this.rawCode = rawCode;
		this.keyCode = keyCode;
		this.keyChar = keyChar;
	}
	
	/**
	 * Instantiates a new native key event.
	 *
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iKeyCode the i key code
	 * @param cKeyChar the c key char
	 * @param iKeyLocation the i key location
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int rawCode, int keyCode, char keyChar, int keyLocation) {
		this(id, when, modifiers, rawCode, keyCode, keyChar);
		
		this.keyLocation = keyLocation;
	}

	public int getRawCode() {
		return this.rawCode;
	}
	
	public void setRawCode(int rawCode) {
		 this.rawCode = rawCode;
	}
	
	public char getKeyChar() {
		return this.keyChar;
	}

	public void setKeyChar(char keyChar) {
		 this.keyChar = keyChar;
	}

	public int getKeyCode() {
		return this.keyCode;
	}
	
	public void setKeyCode(int keyCode) {
		 this.keyCode = keyCode;
	}
	
	public int getKeyLocation() {
		return this.keyLocation;
	}
	
	public static String getKeyText(int keyCode) {
		return KeyEvent.getKeyText(keyCode);
	}
	
	public static String getKeyModifiersText(int modifiers) {
		return KeyEvent.getKeyModifiersText(modifiers);
	}
	

	
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
