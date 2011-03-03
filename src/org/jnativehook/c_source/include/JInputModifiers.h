/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//TODO These should probably be pulled by JNI from the java class.
//Reference: ${JAVA_HOME}/src.zip/java/awt/event/InputEvent.java
#define JK_SHIFT_MASK			1
#define JK_CTRL_MASK			2
#define JK_META_MASK			4
#define JK_ALT_MASK				8

#define JK_BUTTON1_MASK			16;
#define JK_BUTTON2_MASK			32;
#define JK_BUTTON3_MASK			64;
#define JK_BUTTON4_MASK			128;
#define JK_BUTTON5_MASK			256;
