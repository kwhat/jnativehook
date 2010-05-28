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
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeInputEvent;
 
/**
 * An event which indicates that a mouse action occurred on the system.  
 * This event is not limited to a particular components scope or visibility.
 * <p/>
 * 
 * This low-level event is generated natively for:
 * <ul>
 * 	<li>
 * 		Mouse Events
 * 		<ul>
 * 			<li>a mouse button is pressed</li>
 * 			<li>a mouse button is released</li>
 * 			<li><s>a mouse button is clicked (pressed and released)</s></li>
 * 			<li><s>the mouse cursor enters the unobscured part of component's geometry</s></li>
 * 			<li><s>the mouse cursor exits the unobscured part of component's geometry</s></li>
 * 		</ul>
 * 	</li>
 * 	<li>
 * 		Mouse Motion Events
 * 		<ul>
 * 			<li>the mouse is moved
 * 			<li><s>the mouse is dragged</s></li>
 * 		</ul>
 * 	</li>
 * </ul>
 * <p/>
 * 
 * A NativeMouseEvent object is passed to every <code>NativeMouseListener</code> 
 * object which is registered to receive mouse events using the  
 * {@link GlobalScreen#addNativeMouseListener} method. The GlobalScreen 
 * object then dispatches a NativeMouseEvent to each listener.
 * <p/>
 * 
 * A <code>NativeMouseEvent</code> object is also passed to every 
 * <code>NativeMouseMotionListener</code> object which has been registered to 
 * receive mouse motion events using the 
 * {@link GlobalScreen#addNativeMouseListener} method. The GlobalScreen object 
 * then dispatches a NativeMouseEvent to each listener. 
 * <p/>
 * 
 * When a mouse button is clicked, events are generated and sent to the 
 * registered <code>NativeMouseListeners</code>. The state of modifiers can be 
 * retrieved using {@link NativeInputEvent#getModifiers}. All modifiers returned 
 * by {@link NativeInputEvent#getModifiers} reflect the current state of all 
 * modifiers and buttons.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 * 
 * @see GlobalScreen
 * @see NativeMouseListener
 * @see NativeMouseMotionListener
 */
public class NativeMouseEvent extends NativeInputEvent {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -1241061060064904571L;
	
	/**
	 * The native mouse event's x pointer position.
	 * 
	 * @see #getX()
	 */
	private int x;
	
	/**
	 * The native mouse event's y pointer position.
	 * 
	 * @see #getY()
	 */
	private int y;
	
	/**
	 * Indicates which, if any, of the mouse buttons has changed state.
	 *
	 * The only legal values are the following constants:
	 * <code>NOBUTTON</code>,
	 * <code>BUTTON1</code>,
	 * <code>BUTTON2</code>,
	 * <code>BUTTON3</code>,
	 * <code>BUTTON4</code> or
	 * <code>BUTTON5</code>
	 */
	private int button;

	/** The first number in the range of id's used for native mouse events. */
	public static final int NATIVE_MOUSE_FIRST		= 2500;
	
	/** The last number in the range of id's used for native mouse events. */
	public static final int NATIVE_MOUSE_LAST		= 2507;
	
	/**
	 * The native mouse clicked event. This <code>NativeMouseEvent</code>
	 * is not currently implemented.
	 */
	public static final int NATIVE_MOUSE_CLICKED	= NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse pressed event. This <code>NativeMouseEvent</code>
	 * occurs when a mouse button is depressed.
	 */
	public static final int NATIVE_MOUSE_PRESSED	= 1 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse released event. This <code>NativeMouseEvent</code>
	 * occurs when a mouse button is released.
	 */
	public static final int NATIVE_MOUSE_RELEASED	= 2 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse moved event. This <code>NativeMouseEvent</code>
	 * occurs when the mouse pointer changes position.
	 */
	public static final int NATIVE_MOUSE_MOVED		= 3 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse entered event. This <code>NativeMouseEvent</code>
	 * will probably never be implemented.
	 */
	public static final int NATIVE_MOUSE_ENTERED	= 4 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse exited event. This <code>NativeMouseEvent</code>
	 * will probably never be implemented.
	 */
	public static final int NATIVE_MOUSE_EXITED		= 5 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse dragged event. This <code>NativeMouseEvent</code>
	 * is not currently implemented.
	 */
	public static final int NATIVE_MOUSE_DRAGGED	= 6 + NATIVE_MOUSE_FIRST;
	
	/**
	 * The native mouse dragged event. This <code>NativeMouseEvent</code>
	 * is not currently implemented.
	 */
	public static final int NATIVE_MOUSE_WHEEL		= 7 + NATIVE_MOUSE_FIRST; 

	
	/** Indicates no mouse buttons; used by getButton(). */
	public static final int	NOBUTTON	= 0;
	
	/** Indicates mouse button #1; used by getButton(). */
	public static final int	BUTTON1		= 1;
	
	/** Indicates mouse button #2; used by getButton(). */
	public static final int	BUTTON2		= 2;
	
	/** Indicates mouse button #3; used by getButton(). */
	public static final int	BUTTON3		= 3;
	
	/** Indicates mouse button #4; used by getButton(). */
	public static final int	BUTTON4		= 4;
	
	/** Indicates mouse button #5; used by getButton(). */
	public static final int	BUTTON5		= 5;
	
	
	/**
	 * Instantiates a new <code>NativeMouseEvent</code> object.
	 *
	 * @param id an integer that identifies the event
	 * @param when a long integer that gives the time the event occurred
	 * @param modifiers the modifier keys down during event. <code>NativeInputEvent _MASK</code> modifiers should be used as they are not compatible with the extended _DOWN_MASK or the old _MASK InputEvent modifiers.
	 * @param x the x coordinate of the native pointer
	 * @param y the y coordinate of the native pointer
	 * @param button which of the mouse buttons has changed state. NOBUTTON, BUTTON1, BUTTON2, BUTTON3, BUTTON5 or BUTTON5.
	 */
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y, int button) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
		
		this.x = x;
		this.y = y;
		this.button = button;
	}

	/**
	 * Instantiates a new <code>NativeMouseEvent</code> object.
	 *
	 * @param id an integer that identifies the event
	 * @param when a long integer that gives the time the event occurred
	 * @param modifiers the modifier keys down during event. NativeInputEvent _MASK modifiers should be used as they are not compatible with the extended _DOWN_MASK or the old _MASK InputEvent modifiers.
	 * @param x the x coordinate of the native pointer
	 * @param y the y coordinate of the native pointer
	 */
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y) {
		this(id, when, modifiers, x, y, NOBUTTON);
	}

	/**
	 * Returns which, if any, of the mouse buttons has changed state.
	 *
	 * @return one of the following constants:
	 * <code>NOBUTTON</code>,
	 * <code>BUTTON1</code>,
	 * <code>BUTTON2</code>,
	 * <code>BUTTON3</code>,
	 * <code>BUTTON4</code> or
	 * <code>BUTTON5</code>
	 */
	public int getButton() {
		return button;
	}

	/**
	 * Returns the x coordinate of the native pointer.
	 *
	 * @return x an integer indicating horizontal position of the native pointer
	 */
	public int getX() {
		return x;
	}
	
	/**
	 * Returns the y coordinate of the native pointer.
	 *
	 * @return y an integer indicating horizontal position of the native pointer
	 */
	public int getY() {
		return y;
	}
	
	/**
	 * Returns a parameter string identifying the native event.
	 * This method is useful for event-logging and for debugging.
	 *
	 * @return a string identifying the native event and its attributes
	 */
	public String  paramString() {
		String param = "";
		
		switch(getId()) {
			case NATIVE_MOUSE_PRESSED:
				 param += "NATIVE_MOUSE_PRESSED";
			break;
			
			case NATIVE_MOUSE_RELEASED:
				param += "NATIVE_MOUSE_RELEASED";
			break;
			
			case NATIVE_MOUSE_CLICKED:
				param += "NATIVE_MOUSE_CLICKED";
			break;
			
			case NATIVE_MOUSE_ENTERED:
				param += "NATIVE_MOUSE_ENTERED";
			break;
			
			case NATIVE_MOUSE_EXITED:
				param += "NATIVE_MOUSE_EXITED";
			break;
			
			case NATIVE_MOUSE_MOVED:
				param += "NATIVE_MOUSE_MOVED";
			break;
			
			case NATIVE_MOUSE_DRAGGED:
				param += "NATIVE_MOUSE_DRAGGED";
			break;
			
			case NATIVE_MOUSE_WHEEL:
				param += "NATIVE_MOUSE_WHEEL";
			break;
			
			default:
				param += "unknown type";
			break;
		}
		param += ",";
		
		param += "(" + x + "," + y + "),";
		param += "button=" + button + ",";
		
		if (getModifiers() != 0) {
			param += "modifiers=" + getModifiersText(getModifiers()) + ",";
		}
		
		param = param.substring(0, param.length() - 1);
		
		return param;
	}
}
