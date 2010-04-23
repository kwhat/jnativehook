/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.keyboard;

// TODO: Auto-generated Javadoc
/**
 * The Class NativeKeyException.
 */
public class NativeKeyException extends Exception {
	
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -6239432803200521634L;

	/**
	 * Instantiates a new native key exception.
	 */
	public NativeKeyException() {
		super();
	}
	
	/**
	 * Instantiates a new native key exception.
	 *
	 * @param sMessage the s message
	 */
	public NativeKeyException(String message) {
		super(message);
	}
	
	/**
	 * Instantiates a new native key exception.
	 *
	 * @param sMessage the s message
	 * @param objThrowable the obj throwable
	 */
	public NativeKeyException(String message, Throwable cause) {
		super(message, cause);
	}
	
	/**
	 * Instantiates a new native key exception.
	 *
	 * @param objThrowable the obj throwable
	 */
	public NativeKeyException(Throwable cause) {
		super(cause);
	}
}
