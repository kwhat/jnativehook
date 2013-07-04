/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.jnativehook;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class GlobalScreenTest {
	@BeforeClass
	public static void setUpClass() {
	}

	@AfterClass
	public static void tearDownClass() {
	}

	@Before
	public void setUp() {
		GlobalScreen.getInstance();
	}

	@After
	public void tearDown() {
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
}
