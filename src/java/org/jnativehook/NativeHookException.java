/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
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
 * @since	1.0
 * @version	2.0
 */
public class NativeHookException extends Exception {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 8952825837670265527L;

	/** The error code associated with this exception. */
	private int code = NativeHookException.UNSPECIFIED_FAILURE;

	/**
	 * Unspecified failure error code.
	 *
	 * @since	2.0
	 */
	public static final int UNSPECIFIED_FAILURE				= 0x00;

	/**
	 * General failure error code.
	 *
	 * @since	2.0
	 */
	public static final int HOOK_FAILURE					= 0x01;

	/**
	 * X11 open display error code.
	 *
	 * @since	2.0
	 */
	public static final int X11_OPEN_DISPLAY				= 0x20;

	/**
	 * X11 extension XRecord was not found.
	 *
	 * @since	2.0
	 */
	public static final int X11_RECORD_NOT_FOUND			= 0x21;

	/**
	 * X11 extension XRecord could not allocate range.
	 *
	 * @since	2.0
	 */
	public static final int X11_RECORD_ALLOC_RANGE			= 0x22;

	/**
	 * X11 extension XRecord could not create a context.
	 *
	 * @since	2.0
	 */
	public static final int X11_RECORD_CREATE_CONTEXT		= 0x23;


	/**
	 * X11 extension XRecord could not enable the context.
	 */
	public static final int X11_RECORD_ENABLE_CONTEXT		= 0x24;

	/**
	 * X11 extension XRecord could not enable the context.
	 */
	public static final int X11_RECORD_GET_CONTEXT			= 0x25;

	/**
	 * Windows SetWindowsHookEx function failed to register the low level hook.  See log level debug for details.
	 *
	 * @since	2.0
	 */
	public static final int WIN_SET_HOOK					= 0x30;


	/**
	 * Apple access for assistive devices is disabled.
	 */
	public static final int DARWIN_AXAPI_DISABLED			= 0x40;

	/**
	 * Apple could not create an event port.
	 */
	public static final int DARWIN_CREATE_EVENT_PORT		= 0x41;

	/**
	 * Apple could not create a run loop source.
	 */
	public static final int DARWIN_CREATE_RUN_LOOP_SOURCE	= 0x42;

	/**
	 * Apple could not acquire the current run loop.
	 */
	public static final int DARWIN_GET_RUNLOOP				= 0x43;

	/**
	 * Apple could not create an observer.
	 */
	public static final int DARWIN_CREATE_OBSERVER			= 0x44;


	/**
	 * Instantiates a new <code>NativeHookException</code> with UNKNOWN_FAILURE as its error code and null as its
	 * detail message.
	 */
	public NativeHookException() {
		super();
	}


	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified error code and null as its detail message.
	 *
	 * @since 2.0
	 *
	 * @param code The native error code.
	 */
	public NativeHookException(int code) {
		super();

		this.code = code;
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with UNKNOWN_FAILURE as its error code and a specified
	 * detail message.
	 *
	 * @param message The detail message.
	 */
	public NativeHookException(String message) {
		super(message);
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified code and detail message.
	 *
	 * @since 2.0
	 *
	 * @param code The native error code.
	 * @param message The detail message.
	 */
	public NativeHookException(int code, String message) {
		super(message);

		this.code = code;
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified detail message and cause.
	 *
	 * @param message The detail message.
	 * @param cause The cause of the exception. A null value is permitted, and indicates that the cause is unknown.
	 */
	public NativeHookException(String message, Throwable cause) {
		super(message, cause);
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified error code, detail message and cause.
	 *
	 * @since 2.0
	 *
	 * @param code The native error code.
	 * @param message The detail message.
	 * @param cause The cause of the exception. A null value is permitted, and indicates that the cause is unknown.
	 */
	public NativeHookException(int code, String message, Throwable cause) {
		super(message, cause);

		this.code = code;
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified cause.
	 *
	 * @param cause The cause of the exception. A null value is permitted, and indicates that the cause is unknown.
	 */
	public NativeHookException(Throwable cause) {
		super(cause);
	}

	/**
	 * Instantiates a new <code>NativeHookException</code> with a specified error code and cause.
	 *
	 * @param code The native error code.
	 * @param cause The cause of the exception. A null value is permitted, and indicates that the cause is unknown.
	 */
	public NativeHookException(int code, Throwable cause) {
		super(cause);

		this.code = code;
	}

	/**
	 * Get the native error code associated with this exception.
	 *
	 * @since 2.0
	 *
	 * @return The native error code
	 */
	public int getCode() {
		return this.code;
	}
}
