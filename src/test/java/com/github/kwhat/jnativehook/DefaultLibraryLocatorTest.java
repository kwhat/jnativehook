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
import java.io.InputStream;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.FileTime;
import java.util.Iterator;
import java.util.jar.Attributes;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.api.parallel.ResourceLock;
import org.junit.jupiter.api.parallel.Resources;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

@ResourceLock(Resources.SYSTEM_PROPERTIES)
public class DefaultLibraryLocatorTest {
    private final DefaultLibraryLocator locator = new DefaultLibraryLocator();

    @Test
    public void testJrtCodeSource() throws Exception {
        URL jrtUrl = new URL(null, "jrt:/com.github.kwhat.jnativehook", new URLStreamHandler() {
            @Override
            protected URLConnection openConnection(URL url) {
                return null;
            }
        });

        IllegalArgumentException thrown = assertThrows(IllegalArgumentException.class, () -> {
            new File(jrtUrl.toURI());
        });
        assertEquals("URI scheme is not \"file\"", thrown.getMessage());
        assertNull(locator.findClassFile(jrtUrl));
    }

    @Test
    public void testNonFileCodeSource() throws Exception {
        assertNull(locator.findClassFile(null));
        assertNull(locator.findClassFile(new URL("jar:file:/tmp/app.jar!/")));
    }

    @Test
    public void testFileCodeSource() throws Exception {
        File tempFile = File.createTempFile("jnativehook", ".tmp");
        tempFile.deleteOnExit();

        assertEquals(
            tempFile.getCanonicalFile(),
            locator.findClassFile(tempFile.toURI().toURL()).getCanonicalFile()
        );

        File tempDir = new File(System.getProperty("java.io.tmpdir"));
        assertTrue(locator.findClassFile(tempDir.toURI().toURL()).isDirectory());
    }

    @Test
    public void testExtractDirectory() {
        String previous = System.getProperty("jnativehook.lib.path");
        try {
            System.clearProperty("jnativehook.lib.path");

            File tmpdir = new File(System.getProperty("java.io.tmpdir"));
            assertEquals(tmpdir, locator.findLibPath(null));

            File jar = new File(tmpdir, "jnativehook.jar");
            assertEquals(jar.getParentFile(), locator.findLibPath(jar));

            File custom = new File(tmpdir, "jnativehook-lib");
            System.setProperty("jnativehook.lib.path", custom.getPath());
            assertEquals(custom, locator.findLibPath(null));
            assertEquals(custom, locator.findLibPath(jar));

            System.setProperty("jnativehook.lib.path", "");
            assertEquals(new File(""), locator.findLibPath(null));
            assertEquals(new File(""), locator.findLibPath(jar));
        }
        finally {
            if (previous == null) {
                System.clearProperty("jnativehook.lib.path");
            }
            else {
                System.setProperty("jnativehook.lib.path", previous);
            }
        }
    }

    /**
     * Checks extraction names and cache reuse for versioned and unversioned distribution JARs.
     *
     * @param directory temporary directory for fixture JARs and extracted libraries.
     * @throws Exception if fixture creation or library lookup fails.
     * @since 2.3
     */
    @Test
    public void testExtractionNames(@TempDir Path directory) throws Exception {
        String previous = System.getProperty("jnativehook.lib.path");
        Path extracted = Files.createDirectory(directory.resolve("extracted"));
        String nativeName = System.mapLibraryName("JNativeHook").replaceAll("\\.jnilib$", ".dylib");
        String arch = NativeSystem.getArchitecture().toString();
        String resource = "com/github/kwhat/jnativehook/lib/" + NativeSystem.getFamily()
                + '/' + arch + '/' + nativeName;
        String[] contents = {"123456789", "hello", "hello"};
        String[] suffixes = {".cbf43926", ".3610a686", "-2.3.0"};
        Class<?>[] classes = {
            DefaultLibraryLocator.class, GlobalScreen.class, NativeLibraryLocator.class,
            NativeSystem.class, NativeSystem.Arch.class, NativeSystem.Family.class
        };

        try {
            System.setProperty("jnativehook.lib.path", extracted.toString());
            for (int i = 0; i < contents.length; i++) {
                byte[] binary = contents[i].getBytes(StandardCharsets.UTF_8);
                Manifest manifest = new Manifest();
                manifest.getMainAttributes().put(Attributes.Name.MANIFEST_VERSION, "1.0");
                if (i == 2) {
                    manifest.getMainAttributes().put(Attributes.Name.IMPLEMENTATION_VERSION, "2.3.0");
                }
                Path archive = directory.resolve("fixture-" + i + ".jar");
                try (JarOutputStream output = new JarOutputStream(Files.newOutputStream(archive), manifest)) {
                    for (Class<?> type : classes) {
                        String classResource = type.getName().replace('.', '/') + ".class";
                        output.putNextEntry(new JarEntry(classResource));
                        try (InputStream input = type.getResourceAsStream('/' + classResource)) {
                            byte[] buffer = new byte[4096];
                            int size;
                            while ((size = input.read(buffer)) != -1) {
                                output.write(buffer, 0, size);
                            }
                        }
                        output.closeEntry();
                    }
                    output.putNextEntry(new JarEntry(resource));
                    output.write(binary);
                    output.closeEntry();
                }

                // Isolate each JAR so its manifest and native resource determine the result.
                try (URLClassLoader loader = new URLClassLoader(new URL[] {archive.toUri().toURL()}, null)) {
                    Class<?> type = loader.loadClass(DefaultLibraryLocator.class.getName());
                    Object instance = type.getDeclaredConstructor().newInstance();
                    Iterator<?> libraries = (Iterator<?>) type.getMethod("getLibraries").invoke(instance);
                    File library = (File) libraries.next();
                    String expected = nativeName.replaceAll("^(.*)\\.(.*)$",
                            "$1" + suffixes[i] + '.' + arch + ".$2");
                    assertEquals(extracted.resolve(expected).toFile(), library);
                    assertArrayEquals(binary, Files.readAllBytes(library.toPath()));

                    FileTime modified = FileTime.fromMillis(1000000000000L);
                    Files.setLastModifiedTime(library.toPath(), modified);
                    libraries = (Iterator<?>) type.getMethod("getLibraries").invoke(instance);
                    assertEquals(library, libraries.next());
                    assertEquals(modified, Files.getLastModifiedTime(library.toPath()));
                }
            }
        } finally {
            if (previous == null) {
                System.clearProperty("jnativehook.lib.path");
            } else {
                System.setProperty("jnativehook.lib.path", previous);
            }
        }
    }
}
