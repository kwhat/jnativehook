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

import com.github.kwhat.jnativehook.keyboard.NativeKeyListener;
import com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest;
import com.github.kwhat.jnativehook.mouse.NativeMouseListener;
import com.github.kwhat.jnativehook.mouse.NativeMouseMotionListener;
import com.github.kwhat.jnativehook.mouse.NativeMouseWheelListener;
import com.github.kwhat.jnativehook.mouse.listeners.NativeMouseInputListenerTest;
import com.github.kwhat.jnativehook.mouse.listeners.NativeMouseWheelListenerTest;
import org.junit.jupiter.api.Test;
import java.lang.reflect.Field;
import javax.swing.event.EventListenerList;
import static org.junit.jupiter.api.Assertions.fail;

public class GlobalScreenTest {
    @Test
    public void testAddNativeKeyListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeKeyListener listener = new NativeKeyListenerTest();
        GlobalScreen.addNativeKeyListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeKeyListener[] nativeKeyListeners = listeners.getListeners(NativeKeyListener.class);
        for (NativeKeyListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (!found) {
            fail("Could not find the listener after it was added!");
        }

        GlobalScreen.removeNativeKeyListener(listener);
    }

    @Test
    public void testRemoveNativeKeyListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        System.out.println("removeNativeKeyListener");

        NativeKeyListener listener = new NativeKeyListenerTest();
        GlobalScreen.addNativeKeyListener(listener);
        GlobalScreen.removeNativeKeyListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeKeyListener[] nativeKeyListeners = listeners.getListeners(NativeKeyListener.class);
        for (NativeKeyListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (found) {
            fail("Found the listener after it was removed!");
        }
    }

    @Test
    public void testAddNativeMouseListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseListener listener = new NativeMouseInputListenerTest();
        GlobalScreen.addNativeMouseListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseListener[] nativeKeyListeners = listeners.getListeners(NativeMouseListener.class);
        for (NativeMouseListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (!found) {
            fail("Could not find the listener after it was added!");
        }

        GlobalScreen.removeNativeMouseListener(listener);
    }

    @Test
    public void testRemoveNativeMouseListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseListener listener = new NativeMouseInputListenerTest();
        GlobalScreen.addNativeMouseListener(listener);
        GlobalScreen.removeNativeMouseListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseListener[] nativeKeyListeners = listeners.getListeners(NativeMouseListener.class);
        for (NativeMouseListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (found) {
            fail("Found the listener after it was removed!");
        }
    }

    @Test
    public void testAddNativeMouseMotionListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseMotionListener listener = new NativeMouseInputListenerTest();
        GlobalScreen.addNativeMouseMotionListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseMotionListener[] nativeKeyListeners = listeners.getListeners(NativeMouseMotionListener.class);
        for (NativeMouseMotionListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (!found) {
            fail("Could not find the listener after it was added!");
        }

        GlobalScreen.removeNativeMouseMotionListener(listener);
    }

    @Test
    public void testRemoveNativeMouseMotionListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseMotionListener listener = new NativeMouseInputListenerTest();
        GlobalScreen.addNativeMouseMotionListener(listener);
        GlobalScreen.removeNativeMouseMotionListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseMotionListener[] nativeKeyListeners = listeners.getListeners(NativeMouseMotionListener.class);
        for (NativeMouseMotionListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (found) {
            fail("Found the listener after it was removed!");
        }
    }

    @Test
    public void testAddNativeMouseWheelListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseWheelListener listener = new NativeMouseWheelListenerTest();
        GlobalScreen.addNativeMouseWheelListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseWheelListener[] nativeKeyListeners = listeners.getListeners(NativeMouseWheelListener.class);
        for (NativeMouseWheelListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (!found) {
            fail("Could not find the listener after it was added!");
        }

        GlobalScreen.removeNativeMouseWheelListener(listener);
    }

    @Test
    public void testRemoveNativeMouseWheelListener() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        NativeMouseWheelListener listener = new NativeMouseWheelListenerTest();
        GlobalScreen.addNativeMouseWheelListener(listener);
        GlobalScreen.removeNativeMouseWheelListener(listener);

        Field eventListeners = GlobalScreen.class.getDeclaredField("eventListeners");
        eventListeners.setAccessible(true);
        EventListenerList listeners = (EventListenerList) eventListeners.get(GlobalScreen.class);

        boolean found = false;
        NativeMouseWheelListener[] nativeKeyListeners = listeners.getListeners(NativeMouseWheelListener.class);
        for (NativeMouseWheelListener nativeKeyListener : nativeKeyListeners) {
            if (nativeKeyListener.equals(listener)) {
                found = true;
                break;
            }
        }

        if (found) {
            fail("Found the listener after it was removed!");
        }
    }
}
