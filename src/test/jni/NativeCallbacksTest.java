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
public class NativeCallbacksTest {
    /**
     * Delivers an event and a log message from a native-created thread.
     * @since 2.3
     */
    private static native void emit();
    /**
     * Exercises the native variadic logging wrapper.
     * @since 2.3
     */
    private static native void logDirectly();

    /**
     * Checks native-created threads, UTF-8 logs, and recovery after a listener throws.
     * @param args unused command-line arguments.
     * @since 2.3
     */
    public static void main(String[] args) {
        GlobalScreen.setEventDispatcher(new VoidDispatchService());
        AtomicInteger count = new AtomicInteger();
        AtomicReference<String> message = new AtomicReference<>();
        Logger logger = Logger.getLogger("com.github.kwhat.jnativehook");
        logger.setUseParentHandlers(false);
        logger.addHandler(new Handler() {
            public void publish(LogRecord record) { message.set(record.getMessage()); }
            public void flush() { }
            public void close() { }
        });
        NativeMouseMotionListener listener = new NativeMouseMotionListener() {
            public void nativeMouseMoved(NativeMouseEvent event) { count.incrementAndGet(); }
        };
        GlobalScreen.addNativeMouseMotionListener(listener);
        emit();
        if (count.get() != 1 || !"UTF-8: \uD83D\uDE00".equals(message.get())) {
            throw new AssertionError("Native callback or UTF-8 log was lost");
        }
        GlobalScreen.removeNativeMouseMotionListener(listener);
        NativeMouseMotionListener throwing = new NativeMouseMotionListener() {
            public void nativeMouseMoved(NativeMouseEvent event) { throw new IllegalStateException("expected callback failure"); }
        };
        GlobalScreen.addNativeMouseMotionListener(throwing);
        emit();
        GlobalScreen.removeNativeMouseMotionListener(throwing);
        GlobalScreen.addNativeMouseMotionListener(listener);
        emit();
        if (count.get() != 2) {
            throw new AssertionError("Callback failure poisoned later delivery");
        }
        logDirectly();
        if (!"wrapper 42".equals(message.get())) {
            throw new AssertionError("Variadic logger wrapper failed");
        }
        System.out.println("Native callback and logging tests passed");
    }
}
