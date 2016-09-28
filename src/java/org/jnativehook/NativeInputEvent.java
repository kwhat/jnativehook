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
package org.jnativehook;

// Imports.
import java.awt.Toolkit;
import java.util.EventObject;

/**
 * The root event class for all native-level input events.  Input events are
 * delivered to listeners as they are received by the native source. There is
 * no method for listeners or subclasses to prevent delivery of the event to
 * the native system. There is no guarantee that the events will be received by
 * Java before they are delivered natively.
 * <p>
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @since	1.0
 * @version	2.1
 *
 * @see org.jnativehook.keyboard.NativeKeyListener
 * @see org.jnativehook.mouse.NativeMouseListener
 * @see org.jnativehook.mouse.NativeMouseMotionListener
 * @see org.jnativehook.mouse.NativeMouseWheelListener
 */
public class NativeInputEvent extends EventObject {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 2306729722565226621L;

	/** The type of event. */
	private int id;

	/** The platform dependent time the event occured at. */
	private long when;

	/** The modifier keys down during event. */
	private int modifiers;
	
	/** Mask for undocumented behavior.
	 * More information available at:
	 * <a href="https://github.com/kwhat/jnativehook/wiki/Usage#consuming-events-unsupported">wiki/Usage#consuming-events-unsupported</a>
	 * @since 2.0
	 */
	@SuppressWarnings("unused")
	private short reserved;

	/** The left shift key modifier constant.
	 * @since 2.0
	 */
	public static final int SHIFT_L_MASK		= 1 << 0;

	/** The left ctrl key modifier constant. 
	 * @since 2.0
	 */
	public static final int CTRL_L_MASK			= 1 << 1;

	/** The left meta key modifier constant. 
	 * @since 2.0
	 */
	public static final int META_L_MASK			= 1 << 2;

	/** The left alt key modifier constant. 
	 * @since 2.0
	 */
	public static final int ALT_L_MASK			= 1 << 3;

	/** The right shift key modifier constant. 
	 * @since 2.0
	 */
	public static final int SHIFT_R_MASK		= 1 << 4;

	/** The right ctrl key modifier constant. 
	 * @since 2.0
	 */
	public static final int CTRL_R_MASK			= 1 << 5;

	/** The right meta key modifier constant. 
	 * @since 2.0
	 */
	public static final int META_R_MASK			= 1 << 6;

	/** The right alt key modifier constant. 
	 * @since 2.0
	 */
	public static final int ALT_R_MASK			= 1 << 7;
	
	/** Either the right or left shift key modifier constant. */
	public static final int SHIFT_MASK			= SHIFT_L_MASK | SHIFT_R_MASK;
	
	/** Either the right or left ctrl key modifier constant. */
	public static final int CTRL_MASK			= CTRL_L_MASK  | CTRL_R_MASK;
	
	/** Either the right or left meta key modifier constant. */
	public static final int META_MASK			= META_L_MASK  | META_R_MASK;

	/** Either the right or left alt key modifier constant. */
	public static final int ALT_MASK			= ALT_L_MASK   | ALT_R_MASK;
	
	/** The Button1 modifier constant. */
	public static final int BUTTON1_MASK		= 1 << 8;

	/** The Button2 modifier constant. */
	public static final int BUTTON2_MASK		= 1 << 9;

	/** The Button3 modifier constant. */
	public static final int BUTTON3_MASK		= 1 << 10;

	/** The Button4 modifier constant. */
	public static final int BUTTON4_MASK		= 1 << 11;

	/** The Button5 modifier constant. */
	public static final int BUTTON5_MASK		= 1 << 12;

	/** The Number Lock modifier constant. */
	public static final int NUM_LOCK_MASK		= 1 << 13;

	/** The Caps Lock modifier constant. */
	public static final int CAPS_LOCK_MASK		= 1 << 14;

	/** The Scroll Lock modifier constant. */
	public static final int SCROLL_LOCK_MASK	= 1 << 15;


	/**
	 * Instantiates a new native input event.
	 *
	 * @param source The source of the event.
	 * @param id The type of event.
	 * @param modifiers the modifier keys down during event.
	 * <code>NativeInputEvent</code> _MASK modifiers should be used as they are
	 * not compatible with the extended _DOWN_MASK or the old _MASK
	 * <code>InputEvent</code> modifiers.
	 */
	public NativeInputEvent(Class<GlobalScreen> source, int id, int modifiers) {
		super(source);

		this.id = id;
		this.when = 0;
		this.modifiers = modifiers;
		this.reserved = 0x00;
	}

	/**
	 * Gets the event type.
	 *
	 * @return the event type
	 */
	public int getID() {
		return id;
	}

	/**
	 * Gets the platform dependent native interval for chronological event sequencing.
	 *
	 * @return the native timestamp
	 */
	public long getWhen() {
		return when;
	}


	/**
	 * Gets the modifier flags for this event.
	 *
	 * @return the modifier flags
	 */
	public int getModifiers() {
		return this.modifiers;
	}

	/**
	 * Sets the modifier flags for this event.
	 *
	 * @param modifiers the new modifier flags
	 */
	public void setModifiers(int modifiers) {
		this.modifiers = modifiers;
	}

	/**
	 * Sets the reserved flags for this event.
	 * <p>
	 *
	 * Note the use of this method may not be supported by all native platforms.
	 * <p>
	 *
	 * Event propagation support for X11 cannot be provided due to an oversight
	 * in the way that XRecord currently operates.  No public method will be
	 * available until a working cross-platform solution can be provided.
	 * 
	 * @param reserved Non-portable flags for unsupported functionality.
	 * 
	 * @since 1.1
	 */
	@SuppressWarnings("unused")
	private void setReserved(short reserved) {
		this.reserved = reserved;
	}
	
	/**
	 * Gets a <code>String</code> describing the modifier flags, such as
	 * "Button1", or "Ctrl+Alt". These strings can be localized by changing the
	 * awt.properties file.
	 *
	 * @param modifiers a modifier mask describing the modifier keys and mouse
	 * buttons of an event.
	 * @return the modifier mask's textual representation.
	 */
	public static String getModifiersText(int modifiers) {
		StringBuilder param = new StringBuilder(255);

		if ((modifiers & NativeInputEvent.SHIFT_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.shift", "Shift"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.CTRL_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.control", "Ctrl"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.META_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.meta", "Meta"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.ALT_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.alt", "Alt"));
			param.append('+');
		}


		if ((modifiers & NativeInputEvent.BUTTON1_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.button1", "Button1"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.BUTTON2_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.button2", "Button2"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.BUTTON3_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.button3", "Button3"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.BUTTON4_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.button4", "Button4"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.BUTTON5_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.button5", "Button5"));
			param.append('+');
		}


		if ((modifiers & NativeInputEvent.NUM_LOCK_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.numLock", "Num Lock"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.CAPS_LOCK_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.capsLock", "Caps Lock"));
			param.append('+');
		}

		if ((modifiers & NativeInputEvent.SCROLL_LOCK_MASK) != 0) {
			param.append(Toolkit.getProperty("AWT.scrollLock", "Scroll Lock"));
			param.append('+');
		}

		if (param.length() > 0) {
			// Remove the trailing '+'.
			param.deleteCharAt(param.length() - 1);
		}

		return param.toString();
	}

	/**
	 * Gets a <code>String</code> representation of this event. This method is
	 * useful for event-logging and debugging.
	 *
	 * @return a string identifying the event and its attributes
	 */
	public String paramString() {
		StringBuilder param = new StringBuilder(255);

		param.append("id=");
		param.append(getID());
		param.append(',');

		param.append("when=");
		param.append(getWhen());
		param.append(',');

		param.append("mask=");
		param.append(Integer.toBinaryString(getModifiers()));
		param.append(',');

		param.append("modifiers=");
		param.append(getModifiersText(getModifiers()));

		return param.toString();
	}
}
