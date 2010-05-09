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
 * The listener interface for receiving nativeMouse events.
 * The class that is interested in processing a nativeMouse
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addNativeMouseListener<code> method. When
 * the nativeMouse event occurs, that object's appropriate
 * method is invoked.
 *
 * @see NativeMouseEvent
 */
public interface NativeMouseListener extends EventListener {
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	 */
	public void mousePressed(NativeMouseEvent e);
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	 */
	public void mouseReleased(NativeMouseEvent e);
	
	//The following are only included to provide MouseListener compatibility.
	//public void mouseClicked(NativeMouseEvent e);
	//public void mouseEntered(MouseEvent e);
	//public void mouseExited(MouseEvent e);
}
