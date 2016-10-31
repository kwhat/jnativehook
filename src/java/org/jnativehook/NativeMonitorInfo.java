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

/**
 * Object representation of a native monitor layout.
 * <p>
 *
 * The primary display is guaranteed to be at number zero.  No other grantee is made about monitor number for subsequent
 * displays.  All logical displays are guaranteed to have x and y coordinates relative to the primary display.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.1
 * @since	2.1
 *
 * @see  java.util.concurrent.ExecutorService
 * @see  org.jnativehook.GlobalScreen#setEventDispatcher
 */
public class NativeMonitorInfo {
	/**
	 * Display number. The primary display is always zero.
	 */
	private short number;

	/**
	 * Pixel distance relative to the primary display in the x direction.
	 */
	private int x;

	/**
	 * Pixel distance relative to the primary display in the y direction.
	 */
	private int y;

	/**
	 * The horizontal size of the current display.
	 */
	private short width;

	/**
	 * The vertical size of the current display.
	 */
	private short height;

	/**
	 * Instantiates a new monitor info class.
	 * <p>
	 * Note that passing in an invalid ID results in unspecified behavior.
	 *
	 * @param number an integer that identifies the display.
	 * @param x the x coordate of this display.
	 * @param y the y coordate of this display.
	 * @param width the width coordate of this display.
	 * @param height the height coordate of this display.
	 */
	public NativeMonitorInfo(short number, int x, int y, short width, short height) {
		this.number = number;
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}

	/**
	 * Get the logical number of this display.  The primary display will always be zero.
	 *
	 * @return the logical number of this display.
	 */
	public short getNumber() {
		return number;
	}

	/**
	 * Set the logical number for this display.
	 *
	 * @param number the logical number of this display.
	 */
	public void setNumber(short number) {
		this.number = number;
	}

	/**
	 * Returns the x offset of this display.
	 *
	 * @return the x offset in pixels of this display.
	 */
	public int getX() {
		return x;
	}

	/**
	 * Set the x offset of this display.
	 *
	 * @param x the x location of this display.
	 */
	public void setX(int x) {
		this.x = x;
	}

	/**
	 * Returns the y offset of this display.
	 *
	 * @return the y offset in pixels of this display.
	 */
	public int getY() {
		return y;
	}

	/**
	 * Set the y offset of this display.
	 *
	 * @param y the y location of this display.
	 */
	public void setY(int y) {
		this.y = y;
	}

	/**
	 * Returns the width of this native display.
	 *
	 * @return the width in pixels of this display.
	 */
	public short getWidth() {
		return width;
	}

	/**
	 * Set the width in pixel value for this display.
	 *
	 * @param width the pixel width of this monitor.
	 */
	public void setWidth(short width) {
		this.width = width;
	}

	/**
	 * Returns the hight of this naitve monitor.
	 *
	 * @return the hight in pixels of this display.
	 */
	public short getHeight() {
		return height;
	}

	/**
	 * Set the height in pixel value for this display.
	 *
	 * @param height the pixel height of this display.
	 */
	public void setHeight(short height) {
		this.height = height;
	}
}
