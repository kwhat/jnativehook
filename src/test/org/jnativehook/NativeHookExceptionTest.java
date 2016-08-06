/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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
package org.jnativehook;

// Imports.
import org.junit.Test;
import static org.junit.Assert.fail;
import static org.junit.Assert.assertEquals;

public class NativeHookExceptionTest {
	//@Rule
	//public ExpectedException thrown = ExpectedException.none();

	@Test
	public void testNativeHookException() {
		System.out.println("NativeHookException");

		String message = "Test NativeHookException";
		//thrown.expect(NativeHookException.class);
		//thrown.expectMessage(message);

		// Cannot support @rule at this time due to macport junit min version 4.6
		//throw new NativeHookException(message);

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
