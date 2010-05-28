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

import java.awt.Toolkit;
import java.util.EventObject;

public class NativeInputEvent extends EventObject {
	private static final long serialVersionUID = -5581084516392038598L;
	private int id;
	private long when;
	private int modifiers;
	
	//InputEvent Values Used.
	public static int SHIFT_MASK	= 1;
	public static int CTRL_MASK		= 2;
	public static int META_MASK		= 4;
	public static int ALT_MASK		= 8; 
	
	//Custom Values
	public static int BUTTON1_MASK	= 16;
	public static int BUTTON2_MASK	= 32;
	public static int BUTTON3_MASK	= 64;
	public static int BUTTON4_MASK	= 128;
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
	
	public String paramString() {
		return null;
	}
}
