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

/**
 * The listener interface for receiving nativeMouseInput events.
 * The class that is interested in processing a nativeMouseInput
 * event implements this interface, and the object created
 * with that class is registered with a component using the
 * component's <code>addNativeMouseInputListener<code> method. When
 * the nativeMouseInput event occurs, that object's appropriate
 * method is invoked.
 *
 * @see NativeMouseInputEvent
 */
public interface NativeMouseInputListener extends NativeMouseListener, NativeMouseMotionListener {
	
}
