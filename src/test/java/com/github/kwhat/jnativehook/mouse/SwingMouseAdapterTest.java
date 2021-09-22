package com.github.kwhat.jnativehook.mouse;

import org.junit.jupiter.api.Test;

import java.awt.event.MouseEvent;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class SwingMouseAdapterTest {
    @Test
    public void testGetJavaKeyEvent() {
        SwingMouseAdapter adapter = new SwingMouseAdapter();
        NativeMouseEvent nativeEvent = new NativeMouseEvent(
                NativeMouseEvent.NATIVE_MOUSE_MOVED,
                0x00,
                666,
                666,
                0);
        MouseEvent event = adapter.getJavaKeyEvent(nativeEvent);
        assertEquals(event.getID(), MouseEvent.MOUSE_MOVED);
    }
}
