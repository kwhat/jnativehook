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

// Imports

import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.keyboard.listeners.NativeKeyListenerImpl;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;
import org.jnativehook.mouse.listeners.NativeMouseInputListenerImpl;
import org.jnativehook.mouse.listeners.NativeMouseWheelListenerImpl;
import org.junit.Test;

import javax.swing.event.EventListenerList;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Modifier;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.logging.ConsoleHandler;
import java.util.logging.Formatter;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class GlobalScreenTest {
    @Test
    public void testProperties() {
		System.out.println("properties");

		/* Broken FIXME
		assertNotNull("Auto Repeat Rate", System.getProperty("jnativehook.key.repeat.rate"));

		assertNotNull("Auto Repeat Delay", System.getProperty("jnativehook.key.repeat.delay"));

		assertNotNull("Double Click Time", System.getProperty("jnativehook.button.multiclick.iterval"));

		assertNotNull("Pointer Sensitivity", System.getProperty("jnativehook.pointer.sensitivity"));

		assertNotNull("Pointer Acceleration Multiplier", System.getProperty("jnativehook.pointer.acceleration.multiplier"));

		assertNotNull("Pointer Acceleration Threshold", System.getProperty("jnativehook.pointer.acceleration.threshold"));
		*/
    }

	/**
	 * Test of addNativeKeyListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeKeyListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("addNativeKeyListener");

		NativeKeyListener listener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeKeyListener[] nativeKeyListeners = listeners.getListeners(NativeKeyListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (!found) {
			fail("Could not find the listener after it was added!");
		}

		GlobalScreen.removeNativeKeyListener(listener);
	}

	/**
	 * Test of removeNativeKeyListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeKeyListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("removeNativeKeyListener");

		NativeKeyListener listener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(listener);
		GlobalScreen.removeNativeKeyListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeKeyListener[] nativeKeyListeners = listeners.getListeners(NativeKeyListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (found) {
			fail("Found the listener after it was removed!");
		}
	}

	/**
	 * Test of addNativeMouseListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("addNativeMouseListener");

		NativeMouseListener listener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseListener[] nativeKeyListeners = listeners.getListeners(NativeMouseListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (!found) {
			fail("Could not find the listener after it was added!");
		}

		GlobalScreen.removeNativeMouseListener(listener);
	}

	/**
	 * Test of removeNativeMouseListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("removeNativeMouseListener");

		NativeMouseListener listener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseListener(listener);
		GlobalScreen.removeNativeMouseListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseListener[] nativeKeyListeners = listeners.getListeners(NativeMouseListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (found) {
			fail("Found the listener after it was removed!");
		}
	}

	/**
	 * Test of addNativeMouseMotionListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseMotionListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("addNativeMouseMotionListener");

		NativeMouseMotionListener listener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseMotionListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseMotionListener[] nativeKeyListeners = listeners.getListeners(NativeMouseMotionListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (!found) {
			fail("Could not find the listener after it was added!");
		}

		GlobalScreen.removeNativeMouseMotionListener(listener);
	}

	/**
	 * Test of removeNativeMouseMotionListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseMotionListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("removeNativeMouseMotionListener");

		NativeMouseMotionListener listener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseMotionListener(listener);
		GlobalScreen.removeNativeMouseMotionListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseMotionListener[] nativeKeyListeners = listeners.getListeners(NativeMouseMotionListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (found) {
			fail("Found the listener after it was removed!");
		}
	}

	/**
	 * Test of addNativeMouseWheelListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseWheelListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("addNativeMouseWheelListener");


		NativeMouseWheelListener listener = new NativeMouseWheelListenerImpl();
		GlobalScreen.addNativeMouseWheelListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseWheelListener[] nativeKeyListeners = listeners.getListeners(NativeMouseWheelListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (!found) {
			fail("Could not find the listener after it was added!");
		}

		GlobalScreen.removeNativeMouseWheelListener(listener);
	}

	/**
	 * Test of removeNativeMouseWheelListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseWheelListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		System.out.println("removeNativeMouseWheelListener");

		NativeMouseWheelListener listener = new NativeMouseWheelListenerImpl();
		GlobalScreen.addNativeMouseWheelListener(listener);
		GlobalScreen.removeNativeMouseWheelListener(listener);

		Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
		eventListeners.setAccessible(true);
		EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

		boolean found = false;
		NativeMouseWheelListener[] nativeKeyListeners = listeners.getListeners(NativeMouseWheelListener.class);
		for (int i = 0; i < nativeKeyListeners.length && !found; i++) {
			if (nativeKeyListeners[i].equals(listener)) {
				found = true;
			}
		}

		if (found) {
			fail("Found the listener after it was removed!");
		}
	}

	/**
	 * Test of registerNativeHook method, of class GlobalScreen.
	 */
	@Test
	public void testRegisterNativeHook() throws NativeHookException {
		System.out.println("registerNativeHook");

		GlobalScreen.registerNativeHook();
	}

	/**
	 * Test of unregisterNativeHook method, of class GlobalScreen.
	 */
	@Test
	public void testUnregisterNativeHook() throws NativeHookException {
		System.out.println("unregisterNativeHook");

		GlobalScreen.unregisterNativeHook();
	}

	/**
	 * Test of isNativeHookRegistered method, of class GlobalScreen.
	 */
	@Test
	public void testIsNativeHookRegistered() throws NativeHookException {
		System.out.println("isNativeHookRegistered");

		GlobalScreen.registerNativeHook();
		assertTrue(GlobalScreen.isNativeHookRegistered());

		GlobalScreen.unregisterNativeHook();
		assertFalse(GlobalScreen.isNativeHookRegistered());
	}

	/**
	 * Test of dispatchEvent method, of class GlobalScreen.
	 */
	@Test
	public void testPostNativeEvent() throws InterruptedException, NativeHookException {
		System.out.println("dispatchEvent");

		// Setup and event listener.
		NativeKeyListenerImpl keyListener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(keyListener);

		NativeMouseInputListenerImpl mouseListener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseListener(mouseListener);

		NativeMouseWheelListenerImpl wheelListener = new NativeMouseWheelListenerImpl();
		GlobalScreen.addNativeMouseWheelListener(wheelListener);

		// Make sure the native thread is running!
		GlobalScreen.registerNativeHook();

		// Dispatch a key event and check to see if it was sent.
		NativeKeyEvent keyEvent = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00,		// Modifiers
				0x00,		// Raw Code
				NativeKeyEvent.VC_A,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		synchronized (keyListener) {
			GlobalScreen.postNativeEvent(keyEvent);
			keyListener.wait(3000);
			NativeKeyEvent lastEvent = keyListener.getLastEvent();
			assertEquals(keyEvent.getKeyCode(), lastEvent.getKeyCode());
			assertEquals(keyEvent.getRawCode(), lastEvent.getRawCode());
		}

/*
		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseEvent mouseEvent = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_CLICKED,
				System.currentTimeMillis(),
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		synchronized (mouseListener) {
			GlobalScreen.postNativeEvent(mouseEvent);
			mouseListener.wait(3000);
			assertEquals(mouseEvent, mouseListener.getLastEvent());
		}

		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseWheelEvent wheelEvent = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				System.currentTimeMillis(),
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		synchronized (wheelListener) {
			GlobalScreen.postNativeEvent(wheelEvent);
			wheelListener.wait(3000);
			assertEquals(wheelEvent, wheelListener.getLastEvent());
		}
*/
		// Stop the native thread.
		GlobalScreen.unregisterNativeHook();

		// Remove all added listeners.
		GlobalScreen.removeNativeKeyListener(keyListener);
		GlobalScreen.removeNativeMouseListener(mouseListener);
		GlobalScreen.removeNativeMouseWheelListener(wheelListener);
	}

	/**
	 * Test of dispatchEvent method, of class GlobalScreen.
	 */
	@Test
	public void testDispatchEvent() throws InterruptedException, NativeHookException {
		System.out.println("dispatchEvent");

		// Setup and event listener.
		NativeKeyListenerImpl keyListener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(keyListener);

		NativeMouseInputListenerImpl mouseListener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseListener(mouseListener);

		NativeMouseWheelListenerImpl wheelListener = new NativeMouseWheelListenerImpl();
		GlobalScreen.addNativeMouseWheelListener(wheelListener);

		// Make sure the native thread is running!
		GlobalScreen.registerNativeHook();

		// Dispatch a key event and check to see if it was sent.
		NativeKeyEvent keyEvent = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00,		// Modifiers
				0x41,		// Raw Code
				NativeKeyEvent.VC_A,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		synchronized (keyListener) {
			GlobalScreen.dispatchEvent(keyEvent);
			keyListener.wait(3000);
			assertEquals(keyEvent, keyListener.getLastEvent());
		}


		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseEvent mouseEvent = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_CLICKED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		synchronized (mouseListener) {
			GlobalScreen.dispatchEvent(mouseEvent);
			mouseListener.wait(3000);
			assertEquals(mouseEvent, mouseListener.getLastEvent());
		}

		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseWheelEvent wheelEvent = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		synchronized (wheelListener) {
			GlobalScreen.dispatchEvent(wheelEvent);
			wheelListener.wait(3000);
			assertEquals(wheelEvent, wheelListener.getLastEvent());
		}

		// Stop the native thread.
		GlobalScreen.unregisterNativeHook();

		// Remove all added listeners.
		GlobalScreen.removeNativeKeyListener(keyListener);
		GlobalScreen.removeNativeMouseListener(mouseListener);
		GlobalScreen.removeNativeMouseWheelListener(wheelListener);
	}

	/**
	 * Test of processKeyEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessKeyEvent() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, InterruptedException {
		System.out.println("processKeyEvent");

		// Setup and event listener.
		NativeKeyListenerImpl listener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(listener);

		// Dispatch a key event and check to see if it was sent.
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00,		// Modifiers
				0x41,		// Raw Code
				NativeKeyEvent.VC_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		synchronized (listener) {
			GlobalScreen.dispatchEvent(event);
			listener.wait(3000);
			assertEquals(event, listener.getLastEvent());
		}

		GlobalScreen.removeNativeKeyListener(listener);
	}

	/**
	 * Test of processMouseEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessMouseEvent() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, InterruptedException {
		System.out.println("processMouseEvent");

		// Setup and event listener.
		NativeMouseInputListenerImpl listener = new NativeMouseInputListenerImpl();
		GlobalScreen.addNativeMouseListener(listener);

		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_CLICKED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		synchronized (listener) {
			GlobalScreen.dispatchEvent(event);
			listener.wait(3000);
			assertEquals(event, listener.getLastEvent());
		}

		GlobalScreen.removeNativeMouseListener(listener);
	}

	/**
	 * Test of processMouseWheelEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessMouseWheelEvent() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException, InterruptedException {
		System.out.println("processMouseWheelEvent");

		// Setup and event listener.
		NativeMouseWheelListenerImpl listener = new NativeMouseWheelListenerImpl();
		GlobalScreen.addNativeMouseWheelListener(listener);

		// Dispatch a mouse event and check to see if it was sent.
		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		synchronized (listener) {
			GlobalScreen.dispatchEvent(event);
			listener.wait(3000);
			assertEquals(event, listener.getLastEvent());
		}

		GlobalScreen.removeNativeMouseWheelListener(listener);
	}

	/**
	 * Test of startEventDispatcher method, of class GlobalScreen.
	 */
	@Test
	public void testStartEventDispatcher() throws IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		System.out.println("startEventDispatcher");

		Field eventExecutor = GlobalScreen.class.getDeclaredField("eventExecutor");
		eventExecutor.setAccessible(true);
		assertNotNull(eventExecutor.get(GlobalScreen.class));
	}

	/**
	 * Test of stopEventDispatcher method, of class GlobalScreen.
	 */
	/* We cannot force finalize.
	@Test
	public void testStopEventDispatcher() throws IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		System.out.println("stopEventDispatcher");

		GlobalScreen.stopEventDispatcher();

		Field eventExecutor = GlobalScreen.class.getDeclaredField("eventExecutor");
		eventExecutor.setAccessible(true);
		assertNull(eventExecutor.get(GlobalScreen.class));
	}
	*/

	/**
	 * Test for incorrect key conversion.
	 */
	@Test
	public void testIncorrectKeyConversion() throws Exception {
		System.out.println("incorrectKeyConversion");
		// Populate all the virtual key codes from NativeKeyEvent
		HashMap<String, Integer> nativeKeyCodes = new HashMap<String, Integer>();
		Field nativeFields[] = NativeKeyEvent.class.getDeclaredFields();
		for (int i = 0; i < nativeFields.length; i++) {
			String name = nativeFields[i].getName();
			int mod = nativeFields[i].getModifiers();

			if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VC_")) {
				nativeKeyCodes.put(name, nativeFields[i].getInt(null));
			}
		}

		// Convert the key sets to arrays for comparison.
		String[] nativeSet = nativeKeyCodes.keySet().toArray(new String[0]);

		final Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
		logger.setUseParentHandlers(true);
		logger.setLevel(Level.WARNING);

		// Add our custom formatter to a console handler.
		ConsoleHandler handler = new ConsoleHandler();
		handler.setFormatter(new Formatter() {
			@Override
			public String format(LogRecord record) {
				StringBuilder line = new StringBuilder();

				line.append(new Date(record.getMillis()))
						.append(" ")
						.append(record.getLevel().getLocalizedName())
						.append(":\t")
						.append(formatMessage(record));

				if (record.getThrown() != null) {
					try {
						StringWriter sw = new StringWriter();
						PrintWriter pw = new PrintWriter(sw);
						record.getThrown().printStackTrace(pw);
						pw.close();
						line.append(sw.toString());
						sw.close();
					}
					catch (Exception ex) { /* Do Nothing */ }
				}

				return line.toString();
			}
		});

		handler.setLevel(Level.WARNING);
		logger.addHandler(handler);


		// Setup and event listener.
		NativeKeyListenerImpl keyListener = new NativeKeyListenerImpl();
		GlobalScreen.addNativeKeyListener(keyListener);

		//NativeMouseInputListene	rImpl mouseListener = new NativeMouseInputListenerImpl();
		//GlobalScreen.addNativeMouseListener(mouseListener);

		//NativeMouseWheelListenerImpl wheelListener = new NativeMouseWheelListenerImpl();
		//GlobalScreen.addNativeMouseWheelListener(wheelListener);

		// Make sure the native thread is running!
		GlobalScreen.registerNativeHook();


		Iterator<String> keyCodes = nativeKeyCodes.keySet().iterator();
		while (keyCodes.hasNext()) {
			String key = keyCodes.next();
			System.out.println("\n\nTesting " + key);

			// Dispatch a key event and check to see if it was sent.
			NativeKeyEvent keyEvent = new NativeKeyEvent(
					NativeKeyEvent.NATIVE_KEY_PRESSED,
					0x00,        // Modifiers
					0x00,        // Raw Code
					nativeKeyCodes.get(key),
					NativeKeyEvent.CHAR_UNDEFINED,
					NativeKeyEvent.KEY_LOCATION_STANDARD);

			synchronized (keyListener) {
				GlobalScreen.postNativeEvent(keyEvent);
				keyListener.wait(3000);
				NativeKeyEvent lastEvent = keyListener.getLastEvent();
				if (lastEvent.getKeyCode() != NativeKeyEvent.VC_UNDEFINED) {
					assertEquals(keyEvent.getKeyCode(), lastEvent.getKeyCode());
				}
				else {
					System.out.println("Warnign! Undefined.");
				}

			}
		}

		// Stop the native thread.
		GlobalScreen.unregisterNativeHook();

		// Remove all added listeners.
		GlobalScreen.removeNativeKeyListener(keyListener);
		//GlobalScreen.removeNativeMouseListener(mouseListener);
		//GlobalScreen.removeNativeMouseWheelListener(wheelListener);
	}
}
