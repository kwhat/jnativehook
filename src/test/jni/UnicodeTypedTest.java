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
public class UnicodeTypedTest {
    /**
     * Delivers a native typed-character scalar.
     * @param scalar native character value.
     * @since 2.3
     */
    private static native void emit(int scalar);

    /**
     * Checks BMP, supplementary, and invalid native typed-character values.
     * @param args unused command-line arguments.
     * @since 2.3
     */
    public static void main(String[] args) {
        GlobalScreen.setEventDispatcher(new VoidDispatchService());
        StringBuilder typed = new StringBuilder();
        GlobalScreen.addNativeKeyListener(new NativeKeyListener() {
            public void nativeKeyTyped(NativeKeyEvent event) { typed.append(event.getKeyChar()); }
        });
        emit(0x41);
        emit(0x1F600);
        emit(0x110000);
        if (!"A\uD83D\uDE00".equals(typed.toString())) {
            throw new AssertionError("Incorrect UTF-16 typed events");
        }
        System.out.println("Unicode typed-event tests passed");
    }
}
