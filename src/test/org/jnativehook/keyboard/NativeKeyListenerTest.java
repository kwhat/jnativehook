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
package org.jnativehook.keyboard;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class NativeKeyListenerTest {

	public NativeKeyListenerTest() {
	}

	@BeforeClass
	public static void setUpClass() {
	}

	@AfterClass
	public static void tearDownClass() {
	}

	@Before
	public void setUp() {
	}

	@After
	public void tearDown() {
	}

	/**
	 * Test of nativeKeyPressed method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyPressed() {
		System.out.println("nativeKeyPressed");
		NativeKeyEvent nke = null;
		NativeKeyListener instance = new NativeKeyListenerImpl();
		instance.nativeKeyPressed(nke);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of nativeKeyReleased method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyReleased() {
		System.out.println("nativeKeyReleased");
		NativeKeyEvent nke = null;
		NativeKeyListener instance = new NativeKeyListenerImpl();
		instance.nativeKeyReleased(nke);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	/**
	 * Test of nativeKeyTyped method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyTyped() {
		System.out.println("nativeKeyTyped");
		NativeKeyEvent nke = null;
		NativeKeyListener instance = new NativeKeyListenerImpl();
		instance.nativeKeyTyped(nke);
		// TODO review the generated test code and remove the default call to fail.
		fail("The test case is a prototype.");
	}

	public class NativeKeyListenerImpl implements NativeKeyListener {

		public void nativeKeyPressed(NativeKeyEvent nke) {
		}

		public void nativeKeyReleased(NativeKeyEvent nke) {
		}

		public void nativeKeyTyped(NativeKeyEvent nke) {
		}
	}
}
