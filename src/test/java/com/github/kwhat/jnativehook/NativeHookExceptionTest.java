/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
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

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

public class NativeHookExceptionTest {
    @Test
    public void testCode() {
        int code = NativeHookException.HOOK_FAILURE;

        try {
            throw new NativeHookException(code);
        }
        catch (NativeHookException e) {
            assertEquals(code, e.getCode());
        }
        catch (Exception e) {
            fail("Invalid exception type: " + e.getClass());
        }
    }

    @Test
    public void testMessage() {
        String message = "Test NativeHookException";

        try {
            throw new NativeHookException(message);
        }
        catch (NativeHookException e) {
            assertEquals(message, e.getMessage());
        }
        catch (Exception e) {
            fail("Invalid exception type: " + e.getClass());
        }
    }
}
