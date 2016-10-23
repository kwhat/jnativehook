/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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

// Imports.
import org.jnativehook.GlobalScreen;

/**
 * An event which indicates that the mouse wheel was rotated on the system.
 * This event is not limited to a particular component's scope or visibility.
 * <p>
 *
 * A <code>NativeMouseWheelEvent</code> object is passed to every
 * <code>NativeMouseWheelListener</code> object which is registered to receive
 * mouse wheel events using the
 * {@link GlobalScreen#addNativeMouseWheelListener(NativeMouseWheelListener)}
 * method. The <code>GlobalScreen</code> object then dispatches a
 * <code>NativeMouseWheelEvent</code> to each listener.
 * <p>
 *
 * Platforms offer customization of the amount of scrolling that should take
 * place when the mouse wheel is moved. The two most common settings are to
 * scroll a certain number of "units" (commonly lines of text in a text-based
 * component) or an entire "block" (similar to page-up/page-down). The
 * <code>NativeMouseWheelEvent</code> offers methods for conforming to the
 * underlying platform settings. These platform settings can be changed at any
 * time by the user. <code>NativeMouseWheelEvents</code> reflect the most recent
 * settings.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.0
 * @since	1.1
 *
 * @see GlobalScreen
 * @see NativeMouseWheelListener
 */
public class NativeMouseWheelEvent extends NativeMouseEvent {

	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 2112217673594181259L;

	/**
	 * Constant representing scrolling by "units" (like scrolling with the
	 * arrow keys).
	 */
	public static final int	WHEEL_UNIT_SCROLL			= 1;

	/**
	 * Constant representing scrolling by a "block" (like scrolling with
	 * page-up, page-down keys).
	 */
	public static final int	WHEEL_BLOCK_SCROLL			= 2;

	/**
	 * Constant representing scrolling in the vertical direction.
	 * @since 2.1
	 */
	public static final int	WHEEL_VERTICAL_DIRECTION	= 3;

	/**
	 * Constant representing scrolling in the horizontal direction.
	 * @since 2.1
	 */
	public static final int	WHEEL_HORIZONTAL_DIRECTION	= 4;

	/**
	 * Only valid for scrollType WHEEL_UNIT_SCROLL. Indicates number of units
	 * that should be scrolled per click of mouse wheel rotation, based on
	 * platform settings.
	 *
	 * @see #getScrollType
	 * @see #getScrollAmount
	 */
	private int scrollAmount;

	/**
	 * Indicates what sort of scrolling should take place in response to this
	 * event, based on platform settings.  Legal values are:
	 * <ul>
	 * 	<li> WHEEL_UNIT_SCROLL
	 * 	<li> WHEEL_BLOCK_SCROLL
	 * </ul>
	 *
	 * @see #getScrollType
	 */
	private int scrollType;

	/**
	 * Indicates how far the mouse wheel was rotated.
	 *
	 * @see #getWheelRotation
	 */
	private int wheelRotation;

	/**
	 * Indicates how far the mouse wheel was rotated.
	 *
	 * @see #getWheelDirection
	 */
	private int wheelDirection;

	/**
	 * Instantiates a new <code>NativeMouseWheelEvent</code> object with a vertical direction.
	 *
	 * @param id an integer that identifies the native event type.
	 * @param modifiers a modifier mask describing the modifier keys and mouse
	 * buttons active for the event.
	 * <code>NativeInputEvent _MASK</code> modifiers should be used as they are
	 * not compatible with the extended _DOWN_MASK or the old _MASK
	 * <code>InputEvent</code> modifiers.
	 * @param x the x coordinate of the native pointer.
	 * @param y the y coordinate of the native pointer.
	 * @param clickCount the number of button clicks associated with this event.
	 * @param scrollType the type of scrolling which should take place in
	 * response to this event;  valid values are <code>WHEEL_UNIT_SCROLL</code>
	 * and <code>WHEEL_BLOCK_SCROLL</code>.
	 * @param scrollAmount for scrollType <code>WHEEL_UNIT_SCROLL</code>, the
	 * number of units to be scrolled.
	 * @param wheelRotation the amount that the mouse wheel was rotated (the
	 * number of "clicks")
	 *
	 * @see #NativeMouseWheelEvent(int, int, int, int, int, int, int, int, int)
	 * @see NativeMouseEvent#NativeMouseEvent(int, int, int, int, int)
	 */
	public NativeMouseWheelEvent(int id, int modifiers, int x, int y, int clickCount, int scrollType, int scrollAmount, int wheelRotation) {
		this(id, modifiers, x, y, clickCount, scrollType, scrollAmount, wheelRotation, WHEEL_VERTICAL_DIRECTION);
	}

	/**
	 * Instantiates a new <code>NativeMouseWheelEvent</code> object.
	 *
	 * @param id an integer that identifies the native event type.
	 * @param modifiers a modifier mask describing the modifier keys and mouse
	 * buttons active for the event.
	 * <code>NativeInputEvent _MASK</code> modifiers should be used as they are
	 * not compatible with the extended _DOWN_MASK or the old _MASK
	 * <code>InputEvent</code> modifiers.
	 * @param x the x coordinate of the native pointer.
	 * @param y the y coordinate of the native pointer.
	 * @param clickCount the number of button clicks associated with this event.
	 * @param scrollType the type of scrolling which should take place in
	 * response to this event;  valid values are <code>WHEEL_UNIT_SCROLL</code>
	 * and <code>WHEEL_BLOCK_SCROLL</code>.
	 * @param scrollAmount for scrollType <code>WHEEL_UNIT_SCROLL</code>, the
	 * number of units to be scrolled.
	 * @param wheelRotation the amount that the mouse wheel was rotated (the
	 * number of "clicks")
	 * @param wheelDirection the direction of scrolling which should take place in
	 * response to this event;  valid values are <code>WHEEL_VERTICAL_DIRECTION</code>
	 * and <code>WHEEL_HORIZONTAL_DIRECTION</code>.
	 *
	 * @since 2.1
	 * @see NativeMouseEvent#NativeMouseEvent(int, int, int, int, int)
	 */
	public NativeMouseWheelEvent(int id, int modifiers, int x, int y, int clickCount, int scrollType, int scrollAmount, int wheelRotation, int wheelDirection) {
		super(id, modifiers, x, y, clickCount);

		this.scrollType = scrollType;
		this.scrollAmount = scrollAmount;
		this.wheelRotation = wheelRotation;
		this.wheelDirection = wheelDirection;
	}

	/**
	 * Returns the number of units that should be scrolled per
	 * click of mouse wheel rotation.
	 * Only valid if <code>getScrollType</code> returns
	 * <code>NativeMouseWheelEvent.WHEEL_UNIT_SCROLL</code>
	 *
	 * @return number of units to scroll, or an undefined value if
	 *  <code>getScrollType</code> returns
	 *  <code>NativeMouseWheelEvent.WHEEL_BLOCK_SCROLL</code>
	 * @see #getScrollType
	 */
	public int getScrollAmount() {
		return scrollAmount;
	}

	/**
	 * Returns the type of scrolling that should take place in response to this
	 * event.  This is determined by the native platform.  Legal values are:
	 * <ul>
	 * 	<li> MouseWheelEvent.WHEEL_UNIT_SCROLL
	 * 	<li> MouseWheelEvent.WHEEL_BLOCK_SCROLL
	 * </ul>
	 *
	 * @return either NativeMouseWheelEvent.WHEEL_UNIT_SCROLL or
	 *  NativeMouseWheelEvent.WHEEL_BLOCK_SCROLL, depending on the configuration of
	 *  the native platform.
	 * @see java.awt.Adjustable#getUnitIncrement
	 * @see java.awt.Adjustable#getBlockIncrement
	 * @see javax.swing.Scrollable#getScrollableUnitIncrement
	 * @see javax.swing.Scrollable#getScrollableBlockIncrement
	 */
	public int getScrollType() {
		return scrollType;
	}


	/**
	 * Returns the number of "clicks" the mouse wheel was rotated.
	 *
	 * @return negative values if the mouse wheel was rotated up/away from
	 * the user, and positive values if the mouse wheel was rotated down/
	 * toward(s) the user.
	 */
	public int getWheelRotation() {
		return wheelRotation;
	}


	/**
	 * Returns the direction of scrolling that should take place in response to this
	 * event.  Legal values are:
	 * <ul>
	 * 	<li> MouseWheelEvent.WHEEL_VERTICAL_DIRECTION
	 * 	<li> MouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION
	 * </ul>
	 *
	 * @since 2.1
	 * @return either NativeMouseWheelEvent.WHEEL_VERTICAL_DIRECTION or
	 *  NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION.
	 */
	public int getWheelDirection() {
		return wheelDirection;
	}


	/**
	 * Returns a parameter string identifying the native event.
	 * This method is useful for event-logging and debugging.
	 *
	 * @return a string identifying the native event and its attributes.
	 */
	@Override
	public String  paramString() {
		StringBuilder param = new StringBuilder(super.paramString());
		param.append(",scrollType=");

		switch(getScrollType()) {
			case WHEEL_UNIT_SCROLL:
				param.append("WHEEL_UNIT_SCROLL");
				break;

			case WHEEL_BLOCK_SCROLL:
				param.append("WHEEL_BLOCK_SCROLL");
				break;

			default:
				param.append("unknown scroll type");
				break;
		}

		param.append(",scrollAmount=");
		param.append(getScrollAmount());

		param.append(",wheelRotation=");
		param.append(getWheelRotation());

		param.append(",wheelDirection=");
		switch (getWheelDirection()) {
			case WHEEL_VERTICAL_DIRECTION:
				param.append("WHEEL_VERTICAL_DIRECTION");
				break;

			case WHEEL_HORIZONTAL_DIRECTION:
				param.append("WHEEL_HORIZONTAL_DIRECTION");
				break;

			default:
				param.append("unknown scroll direction");
				break;
		}

		return param.toString();
	}
}
