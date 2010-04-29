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
import java.util.EventObject;
import org.jnativehook.GlobalScreen;
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeKeyEvent.
 */
public class NativeKeyEvent extends EventObject {
	
	/**
	 * Instantiates a new native key event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iKeyCode the i key code
	 * @param cKeyChar the c key char
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int keyCode, char keyChar) {
		super(GlobalScreen.getInstance(), id, when, modifiers, keyCode, keyChar);
	}
	
	/**
	 * Instantiates a new native key event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iKeyCode the i key code
	 * @param cKeyChar the c key char
	 * @param iKeyLocation the i key location
	 */
	public NativeKeyEvent(int id, long when, int modifiers, int keyCode, char keyChar, int keyLocation) {
		super(GlobalScreen.getInstance(), id, when, modifiers, keyCode, keyChar, keyLocation);
	}

	public String getKeyCode() {
		// TODO Auto-generated method stub
		return null;
	}

	public static String getKeyText(String keyCode) {
		// TODO Auto-generated method stub
		return null;
	}

	public String getKeyLocation() {
		// TODO Auto-generated method stub
		return null;
	}

	public Object getModifiers() {
		// TODO Auto-generated method stub
		return null;
	}

	public static String getKeyModifiersText(Object modifiers) {
		// TODO Auto-generated method stub
		return null;
	}

}
