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
package org.jnativehook.keyboard;

//Imports
import org.jnativehook.GlobalScreen;

import java.util.EventListener;

/**
 * The listener interface for receiving global <code>NativeKeyEvents</code>.
 * <p>
  * The class that is interested in processing a <code>NativeKeyEvent</code>
 * implements this interface, and the object created with that class is
 * registered with the <code>GlobalScreen</code> using the
 * {@link GlobalScreen#addNativeKeyListener(NativeKeyListener)} method. When the
 * <code>NativeKeyEvent</code> occurs, that object's appropriate method is
 * invoked.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.1
 *
 * @see NativeKeyEvent
 */
public interface NativeKeyListener extends EventListener {
	/**
	 * Invoked when a key has been pressed.
	 *
	 * @param e the native key event.
	 */
	public void nativeKeyPressed(NativeKeyEvent e);

	/**
	 * Invoked when a key has been released.
	 *
	 * @param e the native key event.
	 */
	public void nativeKeyReleased(NativeKeyEvent e);

	/**
	 * Invoked when a key has been typed.
	 *
	 * @param e the native key event.
	 *
	 * @since 1.1
	 */
	public void nativeKeyTyped(NativeKeyEvent e);
}
