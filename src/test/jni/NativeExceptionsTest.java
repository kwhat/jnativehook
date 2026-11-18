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
public class NativeExceptionsTest {
    /**
     * Invokes a native exception-helper scenario.
     * @param mode exception scenario to exercise.
     * @since 2.3
     */
    private static native void fail(int mode);

    /**
     * Checks pending exceptions, full-width codes, and unexpected start/stop failures.
     * @param args unused command-line arguments.
     * @throws Exception if reflective access to the hook thread fails.
     * @since 2.3
     */
    public static void main(String[] args) throws Exception {
        GlobalScreen.getNativeMonitors();
        for (int mode = 0; mode < 3; mode++) {
            try {
                fail(mode);
                throw new AssertionError("Expected exception");
            } catch (IllegalStateException expected) {
                if (mode == 2 || !"original".equals(expected.getMessage())) {
                    throw new AssertionError(expected);
                }
            } catch (Throwable error) {
                if (mode != 2 || !(error instanceof NativeHookException)
                        || ((NativeHookException) error).getCode() != 0x12345) {
                    throw new AssertionError(error);
                }
            }
        }
        Class<?> type = Class.forName("com.github.kwhat.jnativehook.GlobalScreen$NativeHookThread");
        java.lang.reflect.Constructor<?> constructor = type.getDeclaredConstructor();
        constructor.setAccessible(true);
        Object thread = constructor.newInstance();
        for (String name : new String[] {"enable", "disable"}) {
            java.lang.reflect.Method method = type.getDeclaredMethod(name);
            method.setAccessible(true);
            try {
                method.invoke(thread);
                throw new AssertionError("Expected unknown hook error");
            } catch (java.lang.reflect.InvocationTargetException expected) {
                Throwable error = expected.getCause();
                if (!(error instanceof NativeHookException)
                        || ((NativeHookException) error).getCode() != 0x12345) {
                    throw new AssertionError(error);
                }
            }
        }
        System.out.println("Native exception tests passed");
    }
}
