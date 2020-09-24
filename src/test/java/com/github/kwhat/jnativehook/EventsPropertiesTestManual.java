/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2018 Alexander Barker.  All Rights Received.
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
package com.github.kwhat.jnativehook;

import java.util.Scanner;
import java.util.concurrent.Executors;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.github.kwhat.jnativehook.keyboard.NativeKeyEvent;
import com.github.kwhat.jnativehook.keyboard.NativeKeyListener;
import com.github.kwhat.jnativehook.mouse.NativeMouseEvent;
import com.github.kwhat.jnativehook.mouse.NativeMouseInputListener;
import com.github.kwhat.jnativehook.mouse.NativeMouseWheelEvent;
import com.github.kwhat.jnativehook.mouse.NativeMouseWheelListener;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

public class EventsPropertiesTestManual
		implements NativeKeyListener, NativeMouseInputListener, NativeMouseWheelListener {

	private static final Logger LOG;
	static {
		LOG = Logger.getLogger(EventsPropertiesTestManual.class.getCanonicalName());
		LOG.setUseParentHandlers(false);
		LOG.setLevel(Level.ALL);
		ConsoleHandler handler = new ConsoleHandler();
		handler.setLevel(Level.ALL);
		LOG.addHandler(handler);
	}

	private static final NativeSystem.Family OS = NativeSystem.getFamily();

	// Warning if dispatch delay is more than this value
	private static final long MAX_DISPATH_DELLAY_MS = 100;

	private boolean active;
	private AssertionError assertionError;

	private NativeInputEvent prevEvent;
	private long initialWhen;
	private long initialMilis;

	@BeforeClass
	public static void beforeClass() throws NativeHookException {
		GlobalScreen.registerNativeHook();
	}

	@AfterClass
	public static void afterClass() throws NativeHookException {
		GlobalScreen.unregisterNativeHook();
	}

	@Test
	public void testEventsTime() throws InterruptedException {
		try {
			active = true;
			GlobalScreen.addNativeMouseWheelListener(this);
			GlobalScreen.addNativeMouseMotionListener(this);
			GlobalScreen.addNativeMouseListener(this);
			GlobalScreen.addNativeKeyListener(this);
			Executors.newSingleThreadExecutor().execute(new Runnable() {

				public void run() {
					// Allow tester stop testing by pressing <EENTER> in console.
					System.out.println( // NOSONAR (Using system console output here)
							"Press <ENTER> to stop testing native events properties...");
					final Scanner scanner = new Scanner(System.in);
					scanner.nextLine();
					scanner.close();
					active = false;
				}
			});
			while (active) {
				// Simple way to wait for error or stop by user
				Thread.sleep(250);
			}
			if (assertionError != null) {
				throw assertionError;
			}
		} finally {
			active = false;
			GlobalScreen.removeNativeMouseWheelListener(this);
			GlobalScreen.removeNativeMouseMotionListener(this);
			GlobalScreen.removeNativeMouseListener(this);
			GlobalScreen.removeNativeKeyListener(this);
		}
	}

	// synchronized to prevent process event in wrong order
	private synchronized void validateEventTime(final NativeInputEvent e) {
		if (!active) {
			// Prevent from events validation when testing is stopping
			return;
		}
		if (prevEvent != null) {
			final long milis = System.currentTimeMillis();
			final long when = e.getWhen();
			if (prevEvent.getWhen() > when || 
					// when <= 0 usually means property "when" wasn't initialized
					when <= 0) {
// FIXME Error still was happening on Windows eventually/randomly.
// Try figure out cause of it. 
// Usually error was happening when different type of events was compared, 
// f.e., mouse wheel and mouse move was compared by timestamp.
// java.lang.AssertionError: Previous event time is higher than current event time: 58078484 > 58078468
//   prevEvent = id=2503,when=58078484,mask=0,modifiers=NATIVE_MOUSE_MOVED,(1051,310),button=0,clickCount=0
//   event = id=2505,when=58078468,mask=0,modifiers=NATIVE_MOUSE_WHEEL,(1051,310),button=0,clickCount=1,
//	  scrollType=WHEEL_UNIT_SCROLL,scrollAmount=3,wheelRotation=0,wheelDirection=WHEEL_VERTICAL_DIRECTION
				assertionError = new AssertionError(
						"Previous event time is higher than current event time: "
								+ prevEvent.getWhen() + " > " + when 
								+ "\nprevEvent = " + prevEvent.paramString()
								+ "\nevent = " + e.paramString());
				active = false;
			}
			final double diffMilis;
			if (NativeSystem.Family.DARWIN == OS) {
				// On macOS native timestamp was in nanoseconds.
				// Tested using: 
				// * macOS 10.15.3 Catalina.
				diffMilis = Math.abs((milis - initialMilis) - (when - initialWhen) / 1000000.0);
			} else {
				// On Windows and Linux native timestamp was in miliseconds.
				// Tested using: 
				// * Windows 10 VM macOS host (using Parallels Desktop).
				// * Ubuntu 18.04 VM macOS host (using Parallels Desktop).
				diffMilis = Math.abs((milis - initialMilis) - (when - initialWhen));
			}

			if (LOG.isLoggable(Level.FINEST)) {
				LOG.log(Level.FINEST, "Time differnce: {0} [ms]", diffMilis);
			}
			if (diffMilis > MAX_DISPATH_DELLAY_MS) {
				LOG.log(Level.WARNING, "Time differnce is > {0} ms: {1}",
						new Object[] {MAX_DISPATH_DELLAY_MS, diffMilis});
			}
		} else {
			initialWhen = e.getWhen();
			initialMilis = System.currentTimeMillis();
		}
		prevEvent = e;
	}

	public void nativeMouseClicked(NativeMouseEvent e) {
		validateEventTime(e);
	}

	public void nativeMousePressed(NativeMouseEvent e) {
		validateEventTime(e);
	}

	public void nativeMouseReleased(NativeMouseEvent e) {
		validateEventTime(e);
	}

	public void nativeMouseMoved(NativeMouseEvent e) {
		validateEventTime(e);
	}

	public void nativeMouseDragged(NativeMouseEvent e) {
		validateEventTime(e);
	}

	public void nativeMouseWheelMoved(NativeMouseWheelEvent e) {
		validateEventTime(e);
	}

	public void nativeKeyTyped(NativeKeyEvent e) {
		validateEventTime(e);
	}

	public void nativeKeyPressed(NativeKeyEvent e) {
		validateEventTime(e);
	}

	public void nativeKeyReleased(NativeKeyEvent e) {
		validateEventTime(e);
	}
}
