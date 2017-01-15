/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
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

/**
 * Signals that a Native Hook Exception has occurred.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.1
 */
public class NativeHookException extends Exception {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 6199753732102764333L;

	/**
	 * Instantiates a new <code>NativeHookException</code> with null as its
	 * detail message.
	 */
	public NativeHookException() {
		super();
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified
	 * detail message.
	 *
	 * @param message The detail message.
	 */
	public NativeHookException(String message) {
		super(message);
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified
	 * detail message and cause.
	 *
	 * @param message The detail message.
	 * @param cause The cause of the exception. A null value is permitted, and
	 * indicates that the cause is nonexistent or unknown.
	 */
	public NativeHookException(String message, Throwable cause) {
		super(message, cause);
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified
	 * cause.
	 *
	 * @param cause The cause of the exception. A null value is permitted, and
	 * indicates that the cause is nonexistent or unknown.
	 */
	public NativeHookException(Throwable cause) {
		super(cause);
	}
}
