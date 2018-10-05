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
package org.jnativehook.keyboard;

import javafx.event.EventType;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;

/**
 * Adapter to convert NativeKeyEvents to JavaFX KeyEvents.
 * The methods are empty so the super call is obsolete.
 *
 */
public class JavaFXKeyAdapter implements NativeKeyListener{

    @Override
    public void nativeKeyPressed(NativeKeyEvent nativeKeyEvent) {
        this.keyPressed(this.getJavaFXKeyEvent(nativeKeyEvent, KeyEvent.KEY_PRESSED));
    }

    @Override
    public void nativeKeyReleased(NativeKeyEvent nativeKeyEvent) {
        this.keyReleased(this.getJavaFXKeyEvent(nativeKeyEvent, KeyEvent.KEY_RELEASED));
    }

    @Override
    public void nativeKeyTyped(NativeKeyEvent nativeKeyEvent) {
        this.keyTyped(this.getJavaFXKeyEvent(nativeKeyEvent, KeyEvent.KEY_TYPED));
    }


    public void keyPressed(KeyEvent keyEvent){
        // Do Nothing
    }

    public void keyReleased(KeyEvent keyEvent){
        // Do Nothing
    }

    public void keyTyped(KeyEvent keyEvent){
        // Do Nothing
    }


    protected KeyEvent getJavaFXKeyEvent(NativeKeyEvent nativeKeyEvent, EventType<KeyEvent> eventType){

        KeyCode keyCode = KeyCode.UNDEFINED;

        switch (nativeKeyEvent.getKeyCode()) {
            case NativeKeyEvent.VC_ESCAPE:
                keyCode = KeyCode.ESCAPE;
                break;

            // Begin Function Keys
            case NativeKeyEvent.VC_F1:
                keyCode = KeyCode.F1;
                break;

            case NativeKeyEvent.VC_F2:
                keyCode = KeyCode.F2;
                break;

            case NativeKeyEvent.VC_F3:
                keyCode = KeyCode.F3;
                break;

            case NativeKeyEvent.VC_F4:
                keyCode = KeyCode.F4;
                break;

            case NativeKeyEvent.VC_F5:
                keyCode = KeyCode.F5;
                break;

            case NativeKeyEvent.VC_F6:
                keyCode = KeyCode.F6;
                break;

            case NativeKeyEvent.VC_F7:
                keyCode = KeyCode.F7;
                break;

            case NativeKeyEvent.VC_F8:
                keyCode = KeyCode.F8;
                break;

            case NativeKeyEvent.VC_F9:
                keyCode = KeyCode.F9;
                break;

            case NativeKeyEvent.VC_F10:
                keyCode = KeyCode.F10;
                break;

            case NativeKeyEvent.VC_F11:
                keyCode = KeyCode.F11;
                break;

            case NativeKeyEvent.VC_F12:
                keyCode = KeyCode.F12;
                break;

            case NativeKeyEvent.VC_F13:
                keyCode = KeyCode.F13;
                break;

            case NativeKeyEvent.VC_F14:
                keyCode = KeyCode.F14;
                break;

            case NativeKeyEvent.VC_F15:
                keyCode = KeyCode.F15;
                break;

            case NativeKeyEvent.VC_F16:
                keyCode = KeyCode.F16;
                break;

            case NativeKeyEvent.VC_F17:
                keyCode = KeyCode.F17;
                break;

            case NativeKeyEvent.VC_F18:
                keyCode = KeyCode.F18;
                break;

            case NativeKeyEvent.VC_F19:
                keyCode = KeyCode.F19;
                break;
            case NativeKeyEvent.VC_F20:
                keyCode = KeyCode.F20;
                break;

            case NativeKeyEvent.VC_F21:
                keyCode = KeyCode.F21;
                break;

            case NativeKeyEvent.VC_F22:
                keyCode = KeyCode.F22;
                break;

            case NativeKeyEvent.VC_F23:
                keyCode = KeyCode.F23;
                break;

            case NativeKeyEvent.VC_F24:
                keyCode = KeyCode.F24;
                break;
            // End Function Keys


            // Begin Alphanumeric Zone
            case NativeKeyEvent.VC_BACKQUOTE:
                keyCode = KeyCode.BACK_QUOTE;
                break;

            case NativeKeyEvent.VC_1:
                keyCode = KeyCode.DIGIT1;
                break;

            case NativeKeyEvent.VC_2:
                keyCode = KeyCode.DIGIT2;
                break;

            case NativeKeyEvent.VC_3:
                keyCode = KeyCode.DIGIT3;
                break;

            case NativeKeyEvent.VC_4:
                keyCode = KeyCode.DIGIT4;
                break;

            case NativeKeyEvent.VC_5:
                keyCode = KeyCode.DIGIT5;
                break;

            case NativeKeyEvent.VC_6:
                keyCode = KeyCode.DIGIT6;
                break;

            case NativeKeyEvent.VC_7:
                keyCode = KeyCode.DIGIT7;
                break;

            case NativeKeyEvent.VC_8:
                keyCode = KeyCode.DIGIT8;
                break;

            case NativeKeyEvent.VC_9:
                keyCode = KeyCode.DIGIT9;
                break;

            case NativeKeyEvent.VC_0:
                keyCode = KeyCode.DIGIT0;
                break;


            case NativeKeyEvent.VC_MINUS:
                keyCode = KeyCode.MINUS;
                break;

            case NativeKeyEvent.VC_EQUALS:
                keyCode = KeyCode.EQUALS;
                break;

            case NativeKeyEvent.VC_BACKSPACE:
                keyCode = KeyCode.BACK_SPACE;
                break;


            case NativeKeyEvent.VC_TAB:
                keyCode = KeyCode.TAB;
                break;

            case NativeKeyEvent.VC_CAPS_LOCK:
                keyCode = KeyCode.CAPS;
                break;


            case NativeKeyEvent.VC_A:
                keyCode = KeyCode.A;
                break;

            case NativeKeyEvent.VC_B:
                keyCode = KeyCode.B;
                break;

            case NativeKeyEvent.VC_C:
                keyCode = KeyCode.C;
                break;

            case NativeKeyEvent.VC_D:
                keyCode = KeyCode.D;
                break;

            case NativeKeyEvent.VC_E:
                keyCode = KeyCode.E;
                break;

            case NativeKeyEvent.VC_F:
                keyCode = KeyCode.F;
                break;

            case NativeKeyEvent.VC_G:
                keyCode = KeyCode.G;
                break;

            case NativeKeyEvent.VC_H:
                keyCode = KeyCode.H;
                break;

            case NativeKeyEvent.VC_I:
                keyCode = KeyCode.I;
                break;

            case NativeKeyEvent.VC_J:
                keyCode = KeyCode.J;
                break;

            case NativeKeyEvent.VC_K:
                keyCode = KeyCode.K;
                break;

            case NativeKeyEvent.VC_L:
                keyCode = KeyCode.L;
                break;

            case NativeKeyEvent.VC_M:
                keyCode = KeyCode.M;
                break;

            case NativeKeyEvent.VC_N:
                keyCode = KeyCode.N;
                break;

            case NativeKeyEvent.VC_O:
                keyCode = KeyCode.O;
                break;

            case NativeKeyEvent.VC_P:
                keyCode = KeyCode.P;
                break;

            case NativeKeyEvent.VC_Q:
                keyCode = KeyCode.Q;
                break;

            case NativeKeyEvent.VC_R:
                keyCode = KeyCode.R;
                break;

            case NativeKeyEvent.VC_S:
                keyCode = KeyCode.S;
                break;

            case NativeKeyEvent.VC_T:
                keyCode = KeyCode.T;
                break;

            case NativeKeyEvent.VC_U:
                keyCode = KeyCode.U;
                break;

            case NativeKeyEvent.VC_V:
                keyCode = KeyCode.V;
                break;

            case NativeKeyEvent.VC_W:
                keyCode = KeyCode.W;
                break;

            case NativeKeyEvent.VC_X:
                keyCode = KeyCode.X;
                break;

            case NativeKeyEvent.VC_Y:
                keyCode = KeyCode.Y;
                break;

            case NativeKeyEvent.VC_Z:
                keyCode = KeyCode.Z;
                break;


            case NativeKeyEvent.VC_OPEN_BRACKET:
                keyCode = KeyCode.OPEN_BRACKET;
                break;

            case NativeKeyEvent.VC_CLOSE_BRACKET:
                keyCode = KeyCode.CLOSE_BRACKET;
                break;

            case NativeKeyEvent.VC_BACK_SLASH:
                keyCode = KeyCode.BACK_SLASH;
                break;


            case NativeKeyEvent.VC_SEMICOLON:
                keyCode = KeyCode.SEMICOLON;
                break;

            case NativeKeyEvent.VC_QUOTE:
                keyCode = KeyCode.QUOTE;
                break;

            case NativeKeyEvent.VC_ENTER:
                keyCode = KeyCode.ENTER;
                break;


            case NativeKeyEvent.VC_COMMA:
                keyCode = KeyCode.COMMA;
                break;

            case NativeKeyEvent.VC_PERIOD:
                keyCode = KeyCode.PERIOD;
                break;

            case NativeKeyEvent.VC_SLASH:
                keyCode = KeyCode.SLASH;
                break;

            case NativeKeyEvent.VC_SPACE:
                keyCode = KeyCode.SPACE;
                break;
            // End Alphanumeric Zone


            case NativeKeyEvent.VC_PRINTSCREEN:
                keyCode = KeyCode.PRINTSCREEN;
                break;

            case NativeKeyEvent.VC_SCROLL_LOCK:
                keyCode = KeyCode.SCROLL_LOCK;
                break;

            case NativeKeyEvent.VC_PAUSE:
                keyCode = KeyCode.PAUSE;
                break;


            // Begin Edit Key Zone
            case NativeKeyEvent.VC_INSERT:
                keyCode = KeyCode.INSERT;
                break;

            case NativeKeyEvent.VC_DELETE:
                keyCode = KeyCode.DELETE;
                break;

            case NativeKeyEvent.VC_HOME:
                keyCode = KeyCode.HOME;
                break;

            case NativeKeyEvent.VC_END:
                keyCode = KeyCode.END;
                break;

            case NativeKeyEvent.VC_PAGE_UP:
                keyCode = KeyCode.PAGE_UP;
                break;

            case NativeKeyEvent.VC_PAGE_DOWN:
                keyCode = KeyCode.PAGE_DOWN;
                break;
            // End Edit Key Zone


            // Begin Cursor Key Zone
            case NativeKeyEvent.VC_UP:
                keyCode = KeyCode.UP;
                break;
            case NativeKeyEvent.VC_LEFT:
                keyCode = KeyCode.LEFT;
                break;
            case NativeKeyEvent.VC_CLEAR:
                keyCode = KeyCode.CLEAR;
                break;
            case NativeKeyEvent.VC_RIGHT:
                keyCode = KeyCode.RIGHT;
                break;
            case NativeKeyEvent.VC_DOWN:
                keyCode = KeyCode.DOWN;
                break;
            // End Cursor Key Zone


            // Begin Numeric Zone
            case NativeKeyEvent.VC_NUM_LOCK:
                keyCode = KeyCode.NUM_LOCK;
                break;

            case NativeKeyEvent.VC_SEPARATOR:
                keyCode = KeyCode.SEPARATOR;
                break;
            // End Numeric Zone


            // Begin Modifier and Control Keys
            case NativeKeyEvent.VC_SHIFT:
                keyCode = KeyCode.SHIFT;
                break;

            case NativeKeyEvent.VC_CONTROL:
                keyCode = KeyCode.CONTROL;
                break;

            case NativeKeyEvent.VC_ALT:
                keyCode = KeyCode.ALT;
                break;

            case NativeKeyEvent.VC_META:
                keyCode = KeyCode.META;
                break;

            case NativeKeyEvent.VC_CONTEXT_MENU:
                keyCode = KeyCode.CONTEXT_MENU;
                break;
            // End Modifier and Control Keys


			/* Begin Media Control Keys
			case NativeKeyEvent.VC_POWER:
			case NativeKeyEvent.VC_SLEEP:
			case NativeKeyEvent.VC_WAKE:
			case NativeKeyEvent.VC_MEDIA_PLAY:
			case NativeKeyEvent.VC_MEDIA_STOP:
			case NativeKeyEvent.VC_MEDIA_PREVIOUS:
			case NativeKeyEvent.VC_MEDIA_NEXT:
			case NativeKeyEvent.VC_MEDIA_SELECT:
			case NativeKeyEvent.VC_MEDIA_EJECT:
			case NativeKeyEvent.VC_VOLUME_MUTE:
			case NativeKeyEvent.VC_VOLUME_UP:
			case NativeKeyEvent.VC_VOLUME_DOWN:
			case NativeKeyEvent.VC_APP_MAIL:
			case NativeKeyEvent.VC_APP_CALCULATOR:
			case NativeKeyEvent.VC_APP_MUSIC:
			case NativeKeyEvent.VC_APP_PICTURES:
			case NativeKeyEvent.VC_BROWSER_SEARCH:
			case NativeKeyEvent.VC_BROWSER_HOME:
			case NativeKeyEvent.VC_BROWSER_BACK:
			case NativeKeyEvent.VC_BROWSER_FORWARD:
			case NativeKeyEvent.VC_BROWSER_STOP:
			case NativeKeyEvent.VC_BROWSER_REFRESH:
			case NativeKeyEvent.VC_BROWSER_FAVORITES:
			// End Media Control Keys */


            // Begin Japanese Language Keys
            case NativeKeyEvent.VC_KATAKANA:
                keyCode = KeyCode.KATAKANA;
                break;

            case NativeKeyEvent.VC_UNDERSCORE:
                keyCode = KeyCode.UNDERSCORE;
                break;

            //case VC_FURIGANA:

            case NativeKeyEvent.VC_KANJI:
                keyCode = KeyCode.KANJI;
                break;

            case NativeKeyEvent.VC_HIRAGANA:
                keyCode = KeyCode.HIRAGANA;
                break;

            //case VC_YEN:
            // End Japanese Language Keys


            // Begin Sun keyboards
            case NativeKeyEvent.VC_SUN_HELP:
                keyCode = KeyCode.HELP;
                break;

            case NativeKeyEvent.VC_SUN_STOP:
                keyCode = KeyCode.STOP;
                break;

            //case VC_SUN_FRONT:

            //case VC_SUN_OPEN:

            case NativeKeyEvent.VC_SUN_PROPS:
                keyCode = KeyCode.PROPS;
                break;

            case NativeKeyEvent.VC_SUN_FIND:
                keyCode = KeyCode.FIND;
                break;

            case NativeKeyEvent.VC_SUN_AGAIN:
                keyCode = KeyCode.AGAIN;
                break;

            //case NativeKeyEvent.VC_SUN_INSERT:

            case NativeKeyEvent.VC_SUN_COPY:
                keyCode = KeyCode.COPY;
                break;

            case NativeKeyEvent.VC_SUN_CUT:
                keyCode = KeyCode.CUT;
                break;
            // End Sun keyboards
        }


        String unicodeCharacter = KeyEvent.CHAR_UNDEFINED;
        if (eventType.equals(KeyEvent.KEY_TYPED)){
            unicodeCharacter = String.valueOf(nativeKeyEvent.getKeyChar());
        }

        return new KeyEvent(eventType,
                            unicodeCharacter,
                            "",
                            keyCode,
                            false,
                            false,
                            false,
                            false);

    }

}
