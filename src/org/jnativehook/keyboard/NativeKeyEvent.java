/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.keyboard;

//Imports
import java.awt.event.KeyEvent;

import org.jnativehook.GlobalScreen;
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeKeyEvent.
 */
public class NativeKeyEvent extends KeyEvent {
	
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -4226037291462485089L;
	
	/**
	 * Instantiates a new native key event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iKeyCode the i key code
	 * @param cKeyChar the c key char
	 */
	public NativeKeyEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar);
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
	public NativeKeyEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar, int iKeyLocation) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar, iKeyLocation);
	}
}
