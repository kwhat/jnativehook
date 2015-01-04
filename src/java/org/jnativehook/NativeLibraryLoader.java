/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
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

import java.io.File;
import java.util.Iterator;

/**
 * Interface used for native library loading.  This interface should be implemented to alter the way the native library
 * is loaded.
 * <p>
 *
 * @author  Aidas Adomkus (vasiukai@gmail.com)
 * @version	2.0
 * @since	2.0
 *
 * @see org.jnativehook.NativeLibraryLoader
 */
public interface NativeLibraryLoader {

	/**
	 * Perform procedures to interface with the native library. These procedures may include acquiring, unpacking and
	 * loading the library into the Java Virtual Machine.
	 */
	public Iterator<File> getLibraries();
}
