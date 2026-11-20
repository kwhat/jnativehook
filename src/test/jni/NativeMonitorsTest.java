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
public class NativeMonitorsTest {
    /**
     * Checks null lists and signed-origin/unsigned-dimension monitor conversion.
     * @param args unused command-line arguments.
     * @since 2.3
     */
    public static void main(String[] args) {
        if (GlobalScreen.getNativeMonitors().length != 0) {
            throw new AssertionError("Expected empty null screen list");
        }
        for (int i = 0; i < 100; i++) {
            NativeMonitorInfo[] monitors = GlobalScreen.getNativeMonitors();
            if (monitors.length != 1 || monitors[0].getX() != -3840
                    || monitors[0].getWidth() != 40000 || monitors[0].getHeight() != 65535) {
                throw new AssertionError("Monitor data was truncated");
            }
        }
        System.out.println("Native monitor tests passed");
    }
}
