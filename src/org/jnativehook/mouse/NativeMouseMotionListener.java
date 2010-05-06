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


// TODO: Auto-generated Javadoc
/**
 * The listener interface for receiving nativeMouseMotion events.
 * The class that is interested in processing a nativeMouseMotion
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addNativeMouseMotionListener<code> method. When
 * the nativeMouseMotion event occurs, that object's appropriate
 * method is invoked.
 *
 * @see NativeMouseMotionEvent
 */
public interface NativeMouseMotionListener extends EventListener {
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	 */
	/**
	 * Mouse moved.
	 *
	 * @param e the e
	 */
	public void mouseMoved(NativeMouseEvent e);
	
	//The following are only included to provide MouseMotionListener compatibility.
	//public void mouseDragged(MouseEvent e);
}
