/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2026 Alexander Barker.  All Rights Reserved.
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
import com.github.kwhat.jnativehook.*;
import com.github.kwhat.jnativehook.dispatcher.VoidDispatchService;
import com.github.kwhat.jnativehook.keyboard.*;
import com.github.kwhat.jnativehook.mouse.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.logging.*;

/** Standalone JNI regression check against the test-only native backend. */
public class EventValidationTest {
    /**
     * Checks invalid posted events and preservation of Java accessor exceptions.
     * @param args unused command-line arguments.
     * @since 2.3
     */
    public static void main(String[] args) throws NativeHookException {
        GlobalScreen.getNativeMonitors();
        try {
            GlobalScreen.postNativeEvent(new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_TYPED,
                    0, 0, NativeKeyEvent.VC_UNDEFINED, 'x'));
            throw new AssertionError("Expected post failure");
        } catch (Throwable error) {
            check(error instanceof NativeHookException);
        }
        try {
            GlobalScreen.postNativeEvent(null);
            throw new AssertionError("Expected null rejection");
        } catch (NullPointerException expected) { }
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeInputEvent(GlobalScreen.class, NativeKeyEvent.NATIVE_KEY_PRESSED, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL, 0, 0, 0, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(123, 0, 0, 0, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 32768, 0, 0)));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 0, 0, 0) {
                    @Override public int getX() { throw new GetterFailure(); }
                }));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeInputEvent(GlobalScreen.class, 0, 0) {
                    @Override public int getID() { throw new GetterFailure(); }
                }));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_TYPED, 0, 0, 0, 'x') {
                    @Override public char getKeyChar() { throw new GetterFailure(); }
                }));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(Double.NaN)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(Double.POSITIVE_INFINITY)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(32769)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(1e-20)));
        GlobalScreen.postNativeEvent(new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 1, 2, 0));
        GlobalScreen.postNativeEvent(wheel(0.25));
        System.out.println("Native posting validation tests passed");
    }
    /**
     * Creates a horizontal wheel event for posting validation.
     * @param rotation precise wheel steps.
     * @return the wheel event.
     * @since 2.3
     */
    private static NativeMouseWheelEvent wheel(double rotation) {
        return new NativeMouseWheelEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL,
                0, -12, 4, 0, NativeMouseWheelEvent.WHEEL_UNIT_SCROLL, 1, 0,
                NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION, rotation);
    }
    private static class GetterFailure extends RuntimeException { }
    /**
     * Checks that an action raises the expected exception type.
     * @param type expected exception type.
     * @param action action to invoke.
     * @since 2.3
     */
    private static void expect(Class<? extends Throwable> type, ThrowingRunnable action) {
        try {
            action.run();
        } catch (Throwable error) {
            if (type.isInstance(error)) {
                return;
            }
            throw new AssertionError("Expected " + type.getName(), error);
        }
        throw new AssertionError("Expected " + type.getName());
    }

    private interface ThrowingRunnable {
        void run() throws NativeHookException;
    }

    /**
     * Checks a smoke-test condition.
     * @param condition condition that must hold.
     * @since 2.3
     */
    private static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError();
        }
    }

}
