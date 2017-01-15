/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.mouse;

//Imports
import org.jnativehook.GlobalScreen;

import java.util.EventListener;

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
 * @version	1.1
 *
 * @see NativeMouseEvent
 */
public interface NativeMouseListener extends EventListener {
	/**
	 * Invoked when a mouse button has been clicked (pressed and released)
	 * without being moved.
	 *
	 * @param e the native mouse event.
	 */
	public void nativeMouseClicked(NativeMouseEvent e);

	/**
	 * Invoked when a mouse button has been pressed
	 *
	 * @param e the native mouse event.
	 */
	public void nativeMousePressed(NativeMouseEvent e);

	/**
	 * Invoked when a mouse button has been released
	 *
	 * @param e the native mouse event.
	 */
	public void nativeMouseReleased(NativeMouseEvent e);
}
