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
import java.security.CodeSource;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.logging.Logger;
import java.util.zip.CRC32;

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

        ProtectionDomain protectionDomain = GlobalScreen.class.getProtectionDomain();
        CodeSource codeSource = null;
        if (protectionDomain != null) {
            codeSource = protectionDomain.getCodeSource();
        }

        URL classLocation = null;
        if (codeSource != null) {
            classLocation = codeSource.getLocation();
        }

        File classFile = this.findClassFile(classLocation);

        File libFile = null;
        if (classFile == null || classFile.isFile()) {
            // Jar Archive
            String libPath = this.findLibPath(classFile).getPath();

            String version = GlobalScreen.class.getPackage().getImplementationVersion();
            if (version != null) {
                version = '-' + version;
            } else {
                // Modular images may lack package versions, so identify the bundled binary instead.
                CRC32 checksum = new CRC32();
                try (InputStream input = GlobalScreen.class.getResourceAsStream(libResourcePath)) {
                    if (input == null) {
                        throw new IOException("Unable to extract the native library " + libResourcePath);
                    }
                    byte[] buffer = new byte[4 * 1024];
                    int size;
                    while ((size = input.read(buffer)) != -1) {
                        checksum.update(buffer, 0, size);
                    }
                } catch (IOException e) {
                    throw new RuntimeException(e.getMessage(), e);
                }
                version = '.' + String.format(Locale.ROOT, "%08x", checksum.getValue());
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

    /**
     * Returns the code-source file for a <code>file:</code> URL, or null for any other location.
     * <p>
     * A null location or a non-file protocol, including a <code>jrt:</code> modular image and a
     * <code>jar:</code> URL, returns null so the native library is copied out of the module
     * instead of constructing a {@link File} from that URI.
     *
     * @param location code-source location, or null.
     * @return the local file, or null when the location is not a file URL.
     * @since 2.3
     */
    protected File findClassFile(URL location) {
        File path = null;

        if (location != null && location.getProtocol().equals("file")) {
            try {
                path = new File(location.toURI());
            }
            catch (IllegalArgumentException | URISyntaxException e) {
                log.warning(e.getMessage());
                path = new File(location.getPath());
            }
        }

        return path;
    }

    /**
     * Directory the native library is extracted into.
     * <p>
     * Uses <code>jnativehook.lib.path</code> when that property is set, including when it is
     * empty. Otherwise uses the parent of a jar <code>classFile</code>, or
     * <code>java.io.tmpdir</code>.
     *
     * @param classFile jar containing this class, or null.
     * @return extraction directory.
     * @since 2.3
     */
    protected File findLibPath(File classFile) {
        File libPath = null;

        String path = System.getProperty("jnativehook.lib.path");
        if (path != null) {
            libPath = new File(path);
        } else if (classFile != null && !classFile.isDirectory()) {
            libPath = classFile.getParentFile();
        }

        if (libPath == null) {
            libPath = new File(System.getProperty("java.io.tmpdir"));
        }

        return libPath;
    }
}
