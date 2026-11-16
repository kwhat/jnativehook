/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2026 Alexander Barker.  All Rights Reserved.
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
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.Path;
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
     * Creates a locator that finds the appropriate native library and extracts it
     * from the distribution JAR when necessary.
     */
    public DefaultLibraryLocator() {
        super();
    }

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
                Path extracted = null;
                try (InputStream input = GlobalScreen.class.getResourceAsStream(libResourcePath)) {
                    if (input == null) {
                        throw new IOException("Unable to extract the native library " + libResourcePath);
                    }
                    // Publish a complete file so failed copies cannot poison later loads.
                    extracted = Files.createTempFile(libFile.getParentFile().toPath(), libNativeName, ".tmp");
                    Files.copy(input, extracted, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
                    try {
                        Files.move(extracted, libFile.toPath());
                    } catch (FileAlreadyExistsException e) {
                        // Another class loader or process completed extraction first.
                    }
                } catch (IOException e) {
                    throw new RuntimeException(e.getMessage(), e);
                } finally {
                    if (extracted != null) {
                        try {
                            Files.deleteIfExists(extracted);
                        } catch (IOException e) {
                            log.warning(e.getMessage());
                        }
                    }
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
