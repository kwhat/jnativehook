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
}
