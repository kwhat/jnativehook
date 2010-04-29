/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
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
import java.awt.event.MouseEvent;
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeInputEvent;
 
// TODO: Auto-generated Javadoc
/**
 * The Class NativeMouseEvent.
 * 
 * @see MouseEvent
 */
public class NativeMouseEvent extends NativeInputEvent {
	private static final long serialVersionUID = -8191766795595766345L;
	private int x;
	private int y;
	private int button;

	public static final int NATIVE_MOUSE_FIRST = 2500;
	public static final int NATIVE_MOUSE_LAST = 2507;
	public static final int NATIVE_MOUSE_CLICKED = NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_PRESSED = 1 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_RELEASED = 2 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_MOVED = 3 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_ENTERED = 4 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_EXITED = 5 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_DRAGGED = 6 + NATIVE_MOUSE_FIRST;
	public static final int NATIVE_MOUSE_WHEEL = 7 + NATIVE_MOUSE_FIRST; 
	
	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param id the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 */
	//TODO clickCount
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y, int button) {
		super(GlobalScreen.getInstance(), id, when, modifiers);
		
		this.x = x;
		this.y = y;
		this.button = button;
	}

	/**
	 * Instantiates a new native mouse event.
	 *
	 * @param iId the i id
	 * @param iWhen the i when
	 * @param iModifiers the i modifiers
	 * @param iX the i x
	 * @param iY the i y
	 * @param iButton the i button
	 */
	//TODO clickCount
	public NativeMouseEvent(int id, long when, int modifiers, int x, int y) {
		this(id, when, modifiers, x, y, MouseEvent.NOBUTTON);
	}

	public int getButton() {
		return button;
	}

	public int getY() {
		return y;
	}

	public int getX() {
		return x;
	}
	
	public static String getMouseModifiersText(int modifiers) {
		return MouseEvent.getMouseModifiersText(modifiers);
	}
	
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
			param += "modifiers=" + getMouseModifiersText(getModifiers()) + ",";
		}
		
		if (getModifiersEx() != 0) {
			param += "extModifiers=" + getModifiersExText(getModifiersEx()) + ",";
		}
		
		param = param.substring(0, param.length() - 1);
		
		return param;
	}
}
