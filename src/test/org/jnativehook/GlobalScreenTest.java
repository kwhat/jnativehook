/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook;

// Imports
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class GlobalScreenTest {
	@BeforeClass
	public static void setUpClass() {
		// Make sure the singleton is loaded!
		GlobalScreen.getInstance().startEventDispatcher();
	}

	@AfterClass
	public static void tearDownClass() throws Throwable {
		GlobalScreen.getInstance().stopEventDispatcher();
	}

	@Before
	public void setUp() {
		GlobalScreen.getInstance();
		
	}

	@After
	public void tearDown() {
		GlobalScreen.getInstance();
	}

    @Test
    public void testProperties() {
		assertNotNull("Auto Repeat Rate",
				System.getProperty("jnativehook.autoRepeatRate"));

		assertNotNull("Auto Repeat Delay",
				System.getProperty("jnativehook.autoRepeatDelay"));

		assertNotNull("Double Click Time",
				System.getProperty("jnativehook.multiClickInterval"));

		assertNotNull("Pointer Sensitivity",
				System.getProperty("jnativehook.pointerSensitivity"));

		assertNotNull("Pointer Acceleration Multiplier",
				System.getProperty("jnativehook.pointerAccelerationMultiplier"));

		assertNotNull("Pointer Acceleration Threshold",
				System.getProperty("jnativehook.pointerAccelerationThreshold"));
    }

	/**
	 * Test of getInstance method, of class GlobalScreen.
	 */
	@Test
	public void testGetInstance() {
		assertNotNull("Checking getInstance() for null", GlobalScreen.getInstance());
	}

	/**
	 * Test of addNativeKeyListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeKeyListener() {
		NativeKeyListener listener = new KeyListener();
		
		GlobalScreen.getInstance().addNativeKeyListener(listener);
	}

	/**
	 * Test of removeNativeKeyListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeKeyListener() {
		System.out.println("removeNativeKeyListener");
		NativeKeyListener nl = null;
		GlobalScreen instance = null;
		instance.removeNativeKeyListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of addNativeMouseListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseListener() {
		System.out.println("addNativeMouseListener");
		NativeMouseListener nl = null;
		GlobalScreen instance = null;
		instance.addNativeMouseListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of removeNativeMouseListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseListener() {
		System.out.println("removeNativeMouseListener");
		NativeMouseListener nl = null;
		GlobalScreen instance = null;
		instance.removeNativeMouseListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of addNativeMouseMotionListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseMotionListener() {
		System.out.println("addNativeMouseMotionListener");
		NativeMouseMotionListener nl = null;
		GlobalScreen instance = null;
		instance.addNativeMouseMotionListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of removeNativeMouseMotionListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseMotionListener() {
		System.out.println("removeNativeMouseMotionListener");
		NativeMouseMotionListener nl = null;
		GlobalScreen instance = null;
		instance.removeNativeMouseMotionListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of addNativeMouseWheelListener method, of class GlobalScreen.
	 */
	@Test
	public void testAddNativeMouseWheelListener() {
		System.out.println("addNativeMouseWheelListener");
		NativeMouseWheelListener nl = null;
		GlobalScreen instance = null;
		instance.addNativeMouseWheelListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of removeNativeMouseWheelListener method, of class GlobalScreen.
	 */
	@Test
	public void testRemoveNativeMouseWheelListener() {
		System.out.println("removeNativeMouseWheelListener");
		NativeMouseWheelListener nl = null;
		GlobalScreen instance = null;
		instance.removeNativeMouseWheelListener(nl);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of registerNativeHook method, of class GlobalScreen.
	 */
	@Test
	public void testRegisterNativeHook() throws Exception {
		System.out.println("registerNativeHook");
		GlobalScreen.registerNativeHook();
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of unregisterNativeHook method, of class GlobalScreen.
	 */
	@Test
	public void testUnregisterNativeHook() {
		System.out.println("unregisterNativeHook");
		GlobalScreen.unregisterNativeHook();
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of isNativeHookRegistered method, of class GlobalScreen.
	 */
	@Test
	public void testIsNativeHookRegistered() {
		System.out.println("isNativeHookRegistered");
		boolean expResult = false;
		boolean result = GlobalScreen.isNativeHookRegistered();
		assertEquals(expResult, result);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of dispatchEvent method, of class GlobalScreen.
	 */
	@Test
	public void testDispatchEvent() {
		System.out.println("dispatchEvent");
		NativeInputEvent nie = null;
		GlobalScreen instance = null;
		instance.dispatchEvent(nie);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of processKeyEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessKeyEvent() {
		System.out.println("processKeyEvent");
		NativeKeyEvent nke = null;
		GlobalScreen instance = null;
		instance.processKeyEvent(nke);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of processMouseEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessMouseEvent() {
		System.out.println("processMouseEvent");
		NativeMouseEvent nme = null;
		GlobalScreen instance = null;
		instance.processMouseEvent(nme);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of processMouseWheelEvent method, of class GlobalScreen.
	 */
	@Test
	public void testProcessMouseWheelEvent() {
		System.out.println("processMouseWheelEvent");
		NativeMouseWheelEvent nmwe = null;
		GlobalScreen instance = null;
		instance.processMouseWheelEvent(nmwe);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of startEventDispatcher method, of class GlobalScreen.
	 */
	@Test
	public void testStartEventDispatcher() {
		System.out.println("startEventDispatcher");
		GlobalScreen instance = null;
		
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of stopEventDispatcher method, of class GlobalScreen.
	 */
	@Test
	public void testStopEventDispatcher() {
		System.out.println("stopEventDispatcher");
		GlobalScreen instance = null;
		instance.stopEventDispatcher();
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}
	
	private class KeyListener implements NativeKeyListener {

			public void nativeKeyPressed(NativeKeyEvent nke) {
				throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
			}

			public void nativeKeyReleased(NativeKeyEvent nke) {
				throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
			}

			public void nativeKeyTyped(NativeKeyEvent nke) {
				throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
			}
		}
}
