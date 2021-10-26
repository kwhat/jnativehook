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
package com.github.kwhat.jnativehook;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Logger;

/**
 * Default implementation of the <code>NativeLibraryLocator</code> interface.  This will first
 * attempt to load the native library from the java.library.path property.  If that fails, it will
 * attempt to extract a library from the jar based on the host operating system and architecture.
 *
 * @author Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version 2.0
 * @since 2.0
 * @see NativeLibraryLocator
 */
public class DefaultLibraryLocator implements NativeLibraryLocator {
    private static final Logger log = Logger.getLogger(GlobalScreen.class.getPackage().getName());

    /**
     * Perform default procedures to interface with the native library. These procedures include
     * unpacking and loading the library into the Java Virtual Machine.
     */
    public Iterator<File> getLibraries() {
        List<File> libraries = new ArrayList<File>(1);

        String libName = System.getProperty("jnativehook.lib.name", "JNativeHook");

        // Get the package name for the GlobalScreen.
        String basePackage = GlobalScreen.class.getPackage().getName().replace('.', '/');

        String libNativeArch = NativeSystem.getArchitecture().toString().toLowerCase();
        String libNativeName = System
            .mapLibraryName(libName) // Get what the system "thinks" the library name should be.
            .replaceAll("\\.jnilib$", "\\.dylib"); // Hack for OS X JRE 1.6 and earlier.

        // Resource path for the native library.
        String libResourcePath = "/" + basePackage + "/lib/" +
            NativeSystem.getFamily().toString().toLowerCase() +
            '/' + libNativeArch + '/' + libNativeName;

        URL classLocation = GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation();

        File classFile = null;
        try {
            classFile = new File(classLocation.toURI());
        }
        catch (URISyntaxException e) {
            log.warning(e.getMessage());
            classFile = new File(classLocation.getPath());
        }

        File libFile = null;
        if (classFile.isFile()) {
            // Jar Archive
            String libPath = System.getProperty("jnativehook.lib.path", classFile.getParentFile().getPath());

            InputStream resourceInputStream = GlobalScreen.class.getResourceAsStream(libResourcePath);
            if (resourceInputStream == null) {
                throw new RuntimeException("Unable to extract the native library " + libResourcePath + "!\n");
            }

            String version = GlobalScreen.class.getPackage().getImplementationVersion();
            if (version != null) {
                version = '-' + version;
            } else {
                version = "";
            }

            libFile = new File(
                libPath,
                libNativeName.replaceAll("^(.*)\\.(.*)$", "$1" + version + '.' + libNativeArch + ".$2")
            );
            if (!libFile.exists()) {
                try {
                    // Check and see if a copy of the native lib already exists.
                    FileOutputStream libOutputStream = new FileOutputStream(libFile);

                    // Read from the digest stream and write to the file steam.
                    int size;
                    byte[] buffer = new byte[4 * 1024];
                    while ((size = resourceInputStream.read(buffer)) != -1) {
                        libOutputStream.write(buffer, 0, size);
                    }

                    // Close all the streams.
                    resourceInputStream.close();
                    libOutputStream.close();
                }
                catch (IOException e) {
                    throw new RuntimeException(e.getMessage(), e);
                }

                log.fine("Extracted library: " + libFile.getPath() + ".\n");
            }
        }  else {
            // Loose Classes
            libFile = Paths.get(classFile.getAbsolutePath(), libResourcePath.toString()).toFile();
        }

        if (!libFile.exists()) {
            throw new RuntimeException("Unable to locate JNI library at " + libFile.getPath() + "!\n");
        }

        log.fine("Loading library: " + libFile.getPath() + ".\n");
        libraries.add(libFile);

        return libraries.iterator();
    }
}
