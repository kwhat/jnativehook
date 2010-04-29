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
package org.jnativehook.mouse;

//Imports
import java.util.EventObject;

import org.jnativehook.GlobalScreen;
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeMouseEvent.
 * 
 * @see MouseEvent
 */
public class NativeMouseEvent extends EventObject {
	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param id the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 */
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y) {
		super(GlobalScreen.getInstance(), id, when, modifiers, x, y, 1, false);
	}
	
	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 * @param iButton the i button
	 */
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y, int button) {
		super(GlobalScreen.getInstance(), id, when, modifiers, x, y, 1, false, button);
	}

	public String getButton() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getY() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getX() {
		// TODO Auto-generated method stub
		return null;
	}

	public int getModifiers() {
		// TODO Auto-generated method stub
		return 0;
	}
}
