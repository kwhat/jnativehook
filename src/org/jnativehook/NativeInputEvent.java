package org.jnativehook;

import java.awt.event.InputEvent;
import java.util.EventObject;

public class NativeInputEvent extends EventObject {
	private static final long serialVersionUID = -5581084516392038598L;
	private int id;
	private long when;
	private int modifiers;
	
	public NativeInputEvent(GlobalScreen source, int id, long when, int modifiers) {
		super(source);
		
		this.id = id;
		this.when = when;
		this.modifiers = modifiers;
	}

	public int getModifiers() {
		//Return the lower 7 bits of the modifier mask
		return this.modifiers & 0x3F;
	}
	
	public int getModifiersEx() {
		//Return the lower 7 bits of the modifier mask
		return this.modifiers & ~0x3F;
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
}
