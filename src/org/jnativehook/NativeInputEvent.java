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
package org.jnativehook;

//Imports
import java.awt.Toolkit;
import java.util.EventObject;

import org.jnativehook.keyboard.NativeKeyListener;

/**
 * The root event class for all native-level input events.  Input events are 
 * delivered to listeners as they are received by the native source. There is 
 * no method for listeners or subclasses to prevent the events delivery to the 
 * native system.  There is also no guarantee that the events will be received 
 * by java before they are delivered natively.
 * <p/>
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 * 
 * @see NativeKeyListener
 * @see NativeMouseListener
 * @see NativeMouseMotionListener
 */
public class NativeInputEvent extends EventObject {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -5581084516392038598L;
	
	/** The type of event. */
	private int id;
	
	/** The time the event occurred. */
	private long when;
	
	/** The modifier keys down during event. */
	private int modifiers;
	
	/** The Shift key modifier constant. */
	public static int SHIFT_MASK	= 1;
	
	/** The Ctrl key modifier constant. */
	public static int CTRL_MASK		= 2;
	
	/** The Meta key modifier constant. */
	public static int META_MASK		= 4;
	
	/** The Alt key modifier constant. */
	public static int ALT_MASK		= 8; 
	
	/** The Button1 modifier constant. */
	public static int BUTTON1_MASK	= 16;

	/** The Button2 modifier constant. */
	public static int BUTTON2_MASK	= 32;
	
	/** The Button3 modifier constant. */
	public static int BUTTON3_MASK	= 64;
	
	/** The Button4 modifier constant. */
	public static int BUTTON4_MASK	= 128;
	
	/** The Button5 modifier constant. */
	public static int BUTTON5_MASK	= 256;
	
	
	public NativeInputEvent(GlobalScreen source, int id, long when, int modifiers) {
		super(source);
		
		this.id = id;
		this.when = when;
		this.modifiers = modifiers;
	}

	
	public int getModifiers() {
		return this.modifiers;
	}
	
	/**
	 * Gets the key modifiers text.
	 *
	 * @param modifiers - the modifier keys down during event (shift, ctrl, alt, meta).
	 * @return the key modifiers text
	 */
	public static String getModifiersText(int modifiers) {
		String param = "";
		
		if ((modifiers & NativeInputEvent.SHIFT_MASK) != 0) {
			param += Toolkit.getProperty("AWT.shift", "Shift") + "+";
		}
		
		if ((modifiers & NativeInputEvent.CTRL_MASK) != 0) {
			param += Toolkit.getProperty("AWT.control", "Ctrl") + "+";
		}
		
		if ((modifiers & NativeInputEvent.META_MASK) != 0) {
			param += Toolkit.getProperty("AWT.meta", "Meta") + "+";
		}
		
		if ((modifiers & NativeInputEvent.ALT_MASK) != 0) {
			param += Toolkit.getProperty("AWT.alt", "Alt") + "+";
		}
		
		
		
		if ((modifiers & NativeInputEvent.BUTTON1_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button1", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON2_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button2", "Button2") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON3_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button3", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON4_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button4", "Button1") + "+";
		}
		
		if ((modifiers & NativeInputEvent.BUTTON5_MASK) != 0) {
			param += Toolkit.getProperty("AWT.button5", "Button1") + "+";
		}
		
		if (param.length() > 0) {
			//Remove the trailing '+'
			param = param.substring(0, param.length() - 1);
		}
		
		return param;
	}
	
	
	public void setModifiers(int modifiers) {
		this.modifiers = modifiers;
	}
	
	
	
	public int getId() {
		return id;
	}
	
	public long getWhen() {
		return when;
	}
	
	
	//FIXME implement
	public String paramString() {
		return null;
	}
}
