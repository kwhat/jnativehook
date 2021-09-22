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
package com.github.kwhat.jnativehook.mouse;

import com.github.kwhat.jnativehook.AbstractSwingInputAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * Adapter to convert NativeKeyEvents to Java KeyEvents. The methods are empty so the super call is
 * obsolete.
 *
 * @since 2.1
 */
public class SwingMouseAdapter extends AbstractSwingInputAdapter implements NativeMouseListener, MouseListener {

    @Override
    public void nativeMouseClicked(NativeMouseEvent nativeEvent) {
        this.mouseClicked(this.getJavaKeyEvent(nativeEvent));
    }

    @Override
    public void nativeMousePressed(NativeMouseEvent nativeEvent) {
        this.mousePressed(this.getJavaKeyEvent(nativeEvent));
    }

    @Override
    public void nativeMouseReleased(NativeMouseEvent nativeEvent) {
        this.mouseReleased(this.getJavaKeyEvent(nativeEvent));
    }

    @Override
    public void mouseClicked(MouseEvent mouseEvent) {}

    @Override
    public void mousePressed(MouseEvent mouseEvent) {}

    @Override
    public void mouseReleased(MouseEvent mouseEvent) {}

    @Override
    public void mouseEntered(MouseEvent mouseEvent) {}

    @Override
    public void mouseExited(MouseEvent mouseEvent) {}

    protected MouseEvent getJavaKeyEvent(NativeMouseEvent nativeEvent) {
        return new MouseEvent(
            this,
            nativeEvent.getID() - (NativeMouseEvent.NATIVE_MOUSE_FIRST
                - MouseEvent.MOUSE_FIRST),
            System.currentTimeMillis(),
            this.getJavaModifiers(nativeEvent.getModifiers()),
            nativeEvent.getX(),
            nativeEvent.getY(),
            nativeEvent.getClickCount(),
            false,
            nativeEvent.getButton());
    }
}
