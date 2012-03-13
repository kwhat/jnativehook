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
package org.jnativehook.keyboard;

//Imports
import java.util.EventListener;

import org.jnativehook.GlobalScreen;

/**
 * The listener interface for receiving system wide <code>NativeKeyEvents</code>.
 * <p/>
 * 
 * The class that is interested in processing a <code>NativeKeyEvent</code> 
 * implements this interface, and the object created with that class is 
 * registered with the <code>GlobalScreen</code> using the
 * {@link GlobalScreen#addNativeKeyListener} method. When the 
 * <code>NativeKeyEvent</code> occurs, that object's appropriate method is 
 * invoked.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 * 
 * @see NativeKeyEvent
 */
public interface NativeKeyListener extends EventListener {
	/** 
	 * Invoked when a key has been pressed
	 * 
	 * @param e The native key event.
	 */
	public void keyPressed(NativeKeyEvent e);
	
	/** 
	 * Invoked when a key has been released
	 * 
	 * @param e The native key event.
	 */
	public void keyReleased(NativeKeyEvent e);
	
	/* (non-Javadoc)
	 * Currently Unimplemented
	 * 
	 * @param e the native mouse event
	 */
	//public void keyTyped(NativeKeyEvent e);
}
