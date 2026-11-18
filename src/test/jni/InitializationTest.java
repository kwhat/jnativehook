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
public class InitializationTest {
    /**
     * Exercises successful loading or the allocation failure requested by the environment.
     * @param args unused command-line arguments.
     * @since 2.3
     */
    public static void main(String[] args) {
        boolean failure = System.getenv("JNATIVEHOOK_FAIL_ALLOCATION") != null;
        try {
            GlobalScreen.getNativeMonitors();
        } catch (OutOfMemoryError | UnsatisfiedLinkError expected) {
            if (failure) {
                System.out.println("Initialization failure cleaned up");
                return;
            }
            throw expected;
        }
        if (failure) {
            throw new AssertionError("Expected allocation failure");
        }
        System.out.println("Initialization test passed");
    }
}
