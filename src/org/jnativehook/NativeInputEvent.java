package org.jnativehook;

import java.awt.event.InputEvent;
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
	
	public static String getModifiersExText(int modifiers) {
		return InputEvent.getModifiersExText(modifiers);
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
