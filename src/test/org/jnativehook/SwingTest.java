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
import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class SwingTest {
	private static Map<String, Integer> awtKeyCodes, nativeKeyCodes;
	
	@BeforeClass
	public static void setUpClass() {.
		Field test[] = NativeKeyEvent.class.getDeclaredFields();
	
	
		Component component = new JTextField(); //JComponent to be clicked, some panel f.e.
		//MouseEvent event = new MouseEvent(component, MouseEvent.MOUSE_CLICKED, System.currentTimeMillis(), 0, 0, 0, 1, false);
		KeyEvent event = new KeyEvent(component)
		
		java.lang.reflect.Field f = AWTEvent.class.getDeclaredField(“focusManagerIsDispatching”);
		f.setAccessible(true);
		f.set(event, Boolean.TRUE);
		((java.awt.Component)component).dispatchEvent(event);
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
	public void testHack() throws Exception {
		awtKeyCodes = new HashMap<String, Integer>();
		nativeKeyCodes = new HashMap<String, Integer>();
				
		
		Field nativeFields[] = NativeKeyEvent.class.getDeclaredFields();
		for (int i = 0; i < nativeFields.length; i++) {
			String name = nativeFields[i].getName();
			int mod = nativeFields[i].getModifiers();
			
			if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VK_")) {
				nativeKeyCodes.put(name, nativeFields[i].getInt(null));
			}
		}
		
		Field awtFields[] = KeyEvent.class.getDeclaredFields();
		for (int i = 0; i < awtFields.length; i++) {
			String name = awtFields[i].getName();
			int mod = awtFields[i].getModifiers();
			
			if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VK_")) {
				awtKeyCodes.put(name, awtFields[i].getInt(null));
			}
		}
		
		String[] nativeSet = nativeKeyCodes.keySet().toArray(new String[0]);
		String[] awtSet = awtKeyCodes.keySet().toArray(new String[0]);
		
		Arrays.sort(nativeSet);
		Arrays.sort(awtSet);
		
		for (int i = 0, j = 0; i < nativeSet.length && j < awtSet.length; i++, j++) {
			//
			
			if (nativeSet[i].equals(awtSet[j]) != true) {
				int searchPosition;
				if ((searchPosition = Arrays.binarySearch(nativeSet, awtSet[j])) >= 0) {
					
					for (int offset = 0; offset < searchPosition - j; offset++) {
						System.out.println("Extra Native Key: " + nativeSet[i + offset]);
					}
					
					i += searchPosition - i;
				}
				else if ((searchPosition = Arrays.binarySearch(awtSet, nativeSet[i])) >= 0) {
					
					for (int offset = 0; offset < searchPosition - i; offset++) {
						System.out.println("Missing Native Key: " + awtSet[j + offset]);
					}
					
					
					j += searchPosition - j;
				}
			}
			
			// SEND EVENT!
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					JFrame frame = new JFrame();
					frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
					JTextField textField = new JTextField();

					textField.addKeyListener(new KeyListener() {

						@Override
						public void keyTyped(KeyEvent e) {
							System.out.println("keyTyped.");
						}

						@Override
						public void keyPressed(KeyEvent e) {
							System.out.println("keyPressed.");
						}

						@Override
						public void keyReleased(KeyEvent e) {
							System.out.println("keyReleased.");
						}
					});
					/*
					textField.addActionListener(new ActionListener() {
						@Override
						public void actionPerformed(ActionEvent ae) {
							System.out.println("Here..");
						}
					});
					*/
					frame.add(textField);

					frame.pack();
					frame.setSize(100, 50);
					frame.setVisible(true);

					textField.requestFocusInWindow();

					try {
						Robot robot = new Robot();

						robot.keyPress(KeyEvent.VK_ENTER);
						robot.keyRelease(KeyEvent.VK_ENTER);

						System.out.println("ROBOT");
					} catch (AWTException e) {
						e.printStackTrace();
					}
				}
			});
		}
		
		System.out.println("done!");
	}
}
