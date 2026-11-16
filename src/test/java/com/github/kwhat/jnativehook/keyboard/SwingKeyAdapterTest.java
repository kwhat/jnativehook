package com.github.kwhat.jnativehook.keyboard;

import java.awt.event.KeyEvent;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import static org.junit.jupiter.api.Assertions.*;

class SwingKeyAdapterTest {
    static Stream<Arguments> keypadOperators() {
        return Stream.of(
            Arguments.of(NativeKeyEvent.VC_KP_MULTIPLY, KeyEvent.VK_MULTIPLY),
            Arguments.of(NativeKeyEvent.VC_KP_ADD, KeyEvent.VK_ADD),
            Arguments.of(NativeKeyEvent.VC_KP_SUBTRACT, KeyEvent.VK_SUBTRACT),
            Arguments.of(NativeKeyEvent.VC_KP_DIVIDE, KeyEvent.VK_DIVIDE),
            Arguments.of(NativeKeyEvent.VC_KP_EQUALS, KeyEvent.VK_EQUALS),
            Arguments.of(NativeKeyEvent.VC_KP_SEPARATOR, KeyEvent.VK_SEPARATOR));
    }

    @ParameterizedTest
    @MethodSource("keypadOperators")
    void keypadOperatorsConvertToDistinctAwtKeys(int nativeCode, int awtCode) {
        NativeKeyEvent nativeEvent = new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_PRESSED,
            0, 0, nativeCode, NativeKeyEvent.CHAR_UNDEFINED, NativeKeyEvent.KEY_LOCATION_NUMPAD);
        KeyEvent event = new SwingKeyAdapter().getJavaKeyEvent(nativeEvent);
        assertEquals(KeyEvent.KEY_PRESSED, event.getID());
        assertEquals(awtCode, event.getKeyCode());
        assertEquals(KeyEvent.KEY_LOCATION_NUMPAD, event.getKeyLocation());
        assertFalse(NativeKeyEvent.getKeyText(nativeCode).startsWith("Unknown"));
    }

    @Test
    void keypadOperatorCodesCannotCollideWithEachOtherOrDigits() {
        Set<Integer> codes = new HashSet<>();
        keypadOperators().forEach(arguments -> assertTrue(codes.add((Integer) arguments.get()[0])));
        for (int digit : new int[] {NativeKeyEvent.VC_0, NativeKeyEvent.VC_1, NativeKeyEvent.VC_2,
                NativeKeyEvent.VC_3, NativeKeyEvent.VC_4, NativeKeyEvent.VC_5, NativeKeyEvent.VC_6,
                NativeKeyEvent.VC_7, NativeKeyEvent.VC_8, NativeKeyEvent.VC_9}) {
            assertFalse(codes.contains(digit));
        }
    }

    @Test
    void typedCharactersRetainUndefinedKeyCode() {
        KeyEvent event = new SwingKeyAdapter().getJavaKeyEvent(new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_TYPED, 0, 0, NativeKeyEvent.VC_UNDEFINED, '\u03BB'));
        assertEquals(KeyEvent.KEY_TYPED, event.getID());
        assertEquals(KeyEvent.VK_UNDEFINED, event.getKeyCode());
        assertEquals('\u03BB', event.getKeyChar());
        assertEquals(KeyEvent.KEY_LOCATION_UNKNOWN, event.getKeyLocation());
    }
}
