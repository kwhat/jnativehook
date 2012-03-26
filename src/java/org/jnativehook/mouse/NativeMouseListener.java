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
package org.jnativehook.mouse;

//Imports
import java.util.EventListener;
import org.jnativehook.GlobalScreen;

/**
 * The listener interface for receiving systemwide <code>NativeMouseEvents</code>. 
 * (To track native mouse moves, use the <code>NativeMouseMotionListener</code>.)
 * <p>
 * The class that is interested in processing a <code>NativeMouseEvent</code> 
 * implements this interface, and the object created with that class is 
 * registered with the <code>GlobalScreen</code> using the 
 * {@link GlobalScreen#addNativeMouseListener} method. When the 
 * <code>NativeMouseMotion</code> event occurs, that object's appropriate 
 * method is invoked.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @since	1.0
 * 
 * @see NativeMouseEvent
 */
public interface NativeMouseListener extends EventListener {
	/**
	 * Invoked when a mouse button has been clicked (pressed and released)
	 * without being moved.
	 *
	 * @param e The native mouse event.
	 */
	public void mouseClicked(NativeMouseEvent e);

	/** 
	 * Invoked when a mouse button has been pressed
	 * 
	 * @param e The native mouse event.
	 */
	public void mousePressed(NativeMouseEvent e);
	
	/** 
	 * Invoked when a mouse button has been released
	 * 
	 * @param e The native mouse event.
	 */
	public void mouseReleased(NativeMouseEvent e);
}
