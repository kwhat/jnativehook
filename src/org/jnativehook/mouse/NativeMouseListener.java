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
import org.jnativehook.GlobalScreen;

/**
 * The listener interface for receiving system wide native mouse events. 
 * (To track native mouse moves and mouse drags, use the NativeMouseMotionListener.)
 * <p>
 * The class that is interested in processing a <code>NativeMouseEvent</code> 
 * implements this interface, and the object created with that class is 
 * registered with the <code>GlobalScreen</code> using the 
 * {@link GlobalScreen#addNativeMouseListener} method. When the 
 * NativeMouseMotion event occurs, that object's appropriate method is invoked.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 * 
 * @see NativeMouseEvent
 */
public interface NativeMouseListener extends EventListener {
	/** 
	 * Invoked when a mouse button has been pressed
	 * 
	 * @param e the native mouse event
	 */
	public void mousePressed(NativeMouseEvent e);
	
	/** 
	 * Invoked when a mouse button has been released
	 * 
	 * @param e the native mouse event
	 */
	public void mouseReleased(NativeMouseEvent e);
	
	/* (non-Javadoc)
	 * Currently Unimplemented
	 * 
	 * @param e the native mouse event
	 */
	//public void mouseClicked(NativeMouseEvent e);
	
	/* (non-Javadoc)
	 * Currently Unimplemented
	 * 
	 * @param e the native mouse event
	 */
	//public void mouseEntered(MouseEvent e);
	
	/* (non-Javadoc)
	 * Currently Unimplemented
	 * 
	 * @param e the native mouse event
	 */
	//public void mouseExited(MouseEvent e);
}
