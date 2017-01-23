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

import org.jnativehook.AbstractSwingInputAdapter;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * Adapter to convert NativeKeyEvents to Java KeyEvents.
 * The methods are empty so the super call is obsolete.
 *
 * @since 2.1
 */
public class SwingMouseAdapter extends AbstractSwingInputAdapter implements NativeMouseListener, MouseListener {

	public void nativeMouseClicked(NativeMouseEvent nativeEvent) {
		this.mouseClicked(this.getJavaKeyEvent(nativeEvent));
	}

	public void nativeMousePressed(NativeMouseEvent nativeEvent) {
		this.mousePressed(this.getJavaKeyEvent(nativeEvent));
	}

	public void nativeMouseReleased(NativeMouseEvent nativeEvent) {
		this.mousePressed(this.getJavaKeyEvent(nativeEvent));
	}

	public void mouseClicked(MouseEvent mouseEvent) {
		// Do Nothing.
	}

	public void mousePressed(MouseEvent mouseEvent) {
		// Do Nothing.
	}

	public void mouseReleased(MouseEvent mouseEvent) {
		// Do Nothing.
	}

	public void mouseEntered(MouseEvent mouseEvent) {
		// Do Nothing.
	}

	public void mouseExited(MouseEvent mouseEvent) {
		// Do Nothing.
	}

	protected MouseEvent getJavaKeyEvent(NativeMouseEvent nativeEvent) {
		return new MouseEvent(
				this,
				nativeEvent.getID() - (NativeMouseEvent.NATIVE_MOUSE_FIRST - NativeMouseEvent.NATIVE_MOUSE_FIRST),
				System.currentTimeMillis(),
				this.getJavaModifiers(nativeEvent.getModifiers()),
				nativeEvent.getX(),
				nativeEvent.getY(),
				nativeEvent.getClickCount(),
				false,
				nativeEvent.getButton());
	}
}
