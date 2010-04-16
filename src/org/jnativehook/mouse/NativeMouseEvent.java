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
package org.jnativehook.mouse;

//Imports
import java.awt.event.MouseEvent;
import org.jnativehook.GlobalScreen;
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeMouseEvent.
 * 
 * @see MouseEvent
 */
public class NativeMouseEvent extends MouseEvent {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 369524271252805186L;

	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 * @param iClickCount the i click count
	 * @param bPopupTrigger the b popup trigger
	 */
	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iX, int iY, int iClickCount, boolean bPopupTrigger) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iX, iY, iClickCount, bPopupTrigger);
	}
	
	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 * @param iClickCount the i click count
	 * @param bPopupTrigger the b popup trigger
	 * @param iButton the i button
	 */
	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iX, int iY, int iClickCount, boolean bPopupTrigger,  int iButton) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iX, iY, iClickCount, bPopupTrigger, iButton);
	}
}
