package com.github.kwhat.jnativehook.mouse;

import com.github.kwhat.jnativehook.NativeInputEvent;
import java.awt.event.InputEvent;
import java.awt.event.MouseWheelEvent;
import java.util.concurrent.atomic.AtomicReference;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import static org.junit.jupiter.api.Assertions.*;

class SwingMouseWheelAdapterTest {
    @ParameterizedTest
    @CsvSource({"1, 0.25", "1, -0.5", "2, 1.75"})
    void conversionPreservesWheelData(int scrollType, double rotation) {
        NativeMouseWheelEvent nativeEvent = new NativeMouseWheelEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL,
            NativeInputEvent.CTRL_R_MASK, -120, 80, 2, scrollType, 3, 0,
            NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION, rotation);
        MouseWheelEvent event = new SwingMouseWheelAdapter().getJavaMouseWheelEvent(nativeEvent);
        assertEquals(MouseWheelEvent.MOUSE_WHEEL, event.getID());
        assertEquals(-120, event.getX());
        assertEquals(80, event.getY());
        assertEquals(2, event.getClickCount());
        assertEquals(scrollType == NativeMouseWheelEvent.WHEEL_UNIT_SCROLL
            ? MouseWheelEvent.WHEEL_UNIT_SCROLL : MouseWheelEvent.WHEEL_BLOCK_SCROLL, event.getScrollType());
        assertEquals(3, event.getScrollAmount());
        assertEquals(0, event.getWheelRotation());
        assertEquals(rotation, event.getPreciseWheelRotation());
        assertEquals(InputEvent.CTRL_DOWN_MASK, event.getModifiersEx());
    }

    @Test
    void callbackForwardsConvertedWheelEvent() {
        AtomicReference<MouseWheelEvent> received = new AtomicReference<>();
        SwingMouseWheelAdapter adapter = new SwingMouseWheelAdapter() {
            @Override public void mouseWheelMoved(MouseWheelEvent event) { received.set(event); }
        };
        adapter.nativeMouseWheelMoved(new NativeMouseWheelEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL,
            0, 10, 20, NativeMouseWheelEvent.WHEEL_UNIT_SCROLL, 1, -2));
        assertNotNull(received.get());
        assertSame(adapter, received.get().getSource());
        assertEquals(-2.0, received.get().getPreciseWheelRotation());
    }
}
