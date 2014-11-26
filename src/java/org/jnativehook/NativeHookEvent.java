/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
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
package org.jnativehook;

//Imports

/**
 * An event which indicates that a mouse action occurred on the system.
 * This event is not limited to a particular component's scope or visibility.
 * <p>
 *
 * This low-level event is generated natively for:
 * <ul>
 * 	<li>
 * 		Native Mouse Events
 * 		<ul>
 * 			<li>a mouse button is pressed</li>
 * 			<li>a mouse button is released</li>
 * 			<li>the mouse wheel was moved</li>
 * 		</ul>
 * 	</li>
 * 	<li>
 * 		Native Mouse Motion Events
 * 		<ul>
 * 			<li>the mouse is moved
 * 			<li>the mouse is dragged</li>
 * 		</ul>
 * 	</li>
 * </ul>
 * <p>
 *
 * A <code>NativeMouseEvent</code> object is passed to every
 * <code>NativeMouseListener</code> object which is registered to receive mouse
 * events using the {@link org.jnativehook.GlobalScreen#addNativeMouseListener} method. The
 * <code>GlobalScreen</code> object then dispatches a
 * <code>NativeMouseEvent</code> to each listener.
 * <p>
 *
 * A <code>NativeMouseEvent</code> object is also passed to every
 * <code>NativeMouseMotionListener</code> object which has been registered to
 * receive mouse motion events using the
 * {@link org.jnativehook.GlobalScreen#addNativeMouseListener} method. The
 * <code>GlobalScreen</code> object then dispatches a
 * <code>NativeMouseEvent</code> to each listener.
 * <p>
 *
 * When a mouse button is clicked, events are generated and sent to the
 * registered <code>NativeMouseListeners</code>. The state of modifiers can be
 * retrieved using {@link org.jnativehook.NativeInputEvent#getModifiers}. All modifiers returned
 * by {@link org.jnativehook.NativeInputEvent#getModifiers} reflect the current state of all
 * modifiers and buttons.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.3
 * @since	1.3
 *
 * @see org.jnativehook.GlobalScreen
 * @see org.jnativehook.mouse.NativeMouseListener
 * @see org.jnativehook.mouse.NativeMouseMotionListener
 */
public class NativeHookEvent extends NativeInputEvent {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 1L;

	/** The first number in the range of id's used for native mouse events. */
	public static final int NATIVE_HOOK_FIRST		= 2300;

	/** The last number in the range of id's used for native mouse events. */
	public static final int NATIVE_HOOK_LAST		= 2301;

	/**
	 * The native mouse pressed event. This <code>NativeMouseEvent</code>
	 * occurs when a mouse button is pressed and released.
	 */
	public static final int NATIVE_HOOK_ENABLED	= NATIVE_HOOK_FIRST;

	/**
	 * The native mouse pressed event. This <code>NativeMouseEvent</code>
	 * occurs when a mouse button is depressed.
	 */
	public static final int NATIVE_HOOK_DISABLED	= 1 + NATIVE_HOOK_FIRST;

	/**
	 * Instantiates a new <code>NativeMouseEvent</code> object.
	 *
	 * @param id an integer that identifies the native event type.
	 * @param when a long integer that gives the time the event occurred
	 * @param modifiers a modifier mask describing the modifier keys and mouse
	 * buttons active for the event.
	 * <code>NativeInputEvent</code> _MASK modifiers should be used as they are
	 * not compatible with the extended _DOWN_MASK or the old _MASK
	 * <code>InputEvent</code> modifiers.
	 *
	 * @since 1.1
	 */
	public NativeHookEvent(int id, long when, int modifiers) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
	}
}
