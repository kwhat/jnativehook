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
package org.jnativehook.mouse;

import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

/**
 * Adapter to convert NativeKeyEvents to Java KeyEvents.
 * The methods are empty so the super call is obsolete.
 *
 * @since 2.1
 */
public class SwingMouseWheelAdapter extends SwingMouseAdapter implements NativeMouseWheelListener, MouseWheelListener {

	public void nativeMouseWheelMoved(NativeMouseWheelEvent nativeEvent) {
		this.mouseWheelMoved(this.getJavaMouseWheelEvent(nativeEvent));
	}

	public void mouseWheelMoved(MouseWheelEvent mouseWheelEvent) {
		// Do Nothing.
	}

	protected MouseWheelEvent getJavaMouseWheelEvent(NativeMouseWheelEvent nativeEvent) {
		int scrollType = MouseWheelEvent.WHEEL_UNIT_SCROLL;
		if (nativeEvent.getScrollType() == NativeMouseWheelEvent.WHEEL_BLOCK_SCROLL) {
			scrollType = MouseWheelEvent.WHEEL_BLOCK_SCROLL;
		}

		return new MouseWheelEvent(
				this,
				nativeEvent.getID() - (NativeMouseWheelEvent.NATIVE_MOUSE_FIRST - NativeMouseWheelEvent.NATIVE_MOUSE_FIRST),
				System.currentTimeMillis(),
				this.getJavaModifiers(nativeEvent.getModifiers()),
				nativeEvent.getX(),
				nativeEvent.getY(),
				nativeEvent.getClickCount(),
				false,
				scrollType,
				nativeEvent.getScrollAmount(),
				nativeEvent.getWheelRotation());
	}
}
