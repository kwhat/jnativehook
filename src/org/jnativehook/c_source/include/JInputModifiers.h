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
//Refernce: ${JAVA_HOME}/src.zip/java/awt/event/InputEvent.java

#define JK_SHIFT_MASK			1 << 0
#define JK_CTRL_MASK			1 << 1
#define JK_META_MASK			1 << 2
#define JK_ALT_MASK				1 << 3
#define JK_BUTTON1_MASK			1 << 4
#define JK_BUTTON2_MASK			JK_ALT_MASK
#define JK_BUTTON3_MASK			JK_META_MASK
//FIXME Where are button 4 and 5
