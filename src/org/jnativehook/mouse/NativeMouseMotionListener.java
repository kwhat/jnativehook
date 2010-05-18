/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
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
import java.util.EventListener;

/**
 * The listener interface for receiving native mouse motion events. 
 * (For clicks and other mouse events, use the NativeMouseListener.)
 * <p>
 * The class that is interested in processing a nativeMouseMotion
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addNativeMouseMotionListener<code> method. When
 * the nativeMouseMotion event occurs, that object's appropriate
 * method is invoked.
 *
 * @author	Alex Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	0.9
 * @since	1.0
 * @see NativeMouseMotionEvent
 */
public interface NativeMouseMotionListener extends EventListener {
	
	/**
	 *  Invoked when the mouse has been moved.
	 *
	 * @param e the native mouse event
	 */
	public void mouseMoved(NativeMouseEvent e);
	
	
	/**
	 * Note Implemented.
	 *
	 * @param e the native mouse event
	 */
	//public void mouseDragged(MouseEvent e);
}
