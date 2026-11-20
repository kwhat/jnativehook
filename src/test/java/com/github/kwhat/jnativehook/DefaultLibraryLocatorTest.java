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

import org.junit.jupiter.api.Test;
import java.io.File;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class DefaultLibraryLocatorTest {
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
        assertNull(DefaultLibraryLocator.codeSourceFile(jrtUrl));
    }

    @Test
    public void testNonFileCodeSource() throws Exception {
        assertNull(DefaultLibraryLocator.codeSourceFile(null));
        assertNull(DefaultLibraryLocator.codeSourceFile(new URL("jar:file:/tmp/app.jar!/")));
    }

    @Test
    public void testFileCodeSource() throws Exception {
        File tempFile = File.createTempFile("jnativehook", ".tmp");
        tempFile.deleteOnExit();

        assertEquals(
            tempFile.getCanonicalFile(),
            DefaultLibraryLocator.codeSourceFile(tempFile.toURI().toURL()).getCanonicalFile()
        );

        File tempDir = new File(System.getProperty("java.io.tmpdir"));
        assertTrue(DefaultLibraryLocator.codeSourceFile(tempDir.toURI().toURL()).isDirectory());
    }

    @Test
    public void testExtractDirectory() {
        String previous = System.getProperty("jnativehook.lib.path");
        try {
            System.clearProperty("jnativehook.lib.path");

            File tmpdir = new File(System.getProperty("java.io.tmpdir"));
            assertEquals(tmpdir, DefaultLibraryLocator.extractDirectory(null));

            File jar = new File(tmpdir, "jnativehook.jar");
            assertEquals(jar.getParentFile(), DefaultLibraryLocator.extractDirectory(jar));

            File custom = new File(tmpdir, "jnativehook-lib");
            System.setProperty("jnativehook.lib.path", custom.getPath());
            assertEquals(custom, DefaultLibraryLocator.extractDirectory(null));
            assertEquals(custom, DefaultLibraryLocator.extractDirectory(jar));

            System.setProperty("jnativehook.lib.path", "");
            assertEquals(new File(""), DefaultLibraryLocator.extractDirectory(null));
            assertEquals(new File(""), DefaultLibraryLocator.extractDirectory(jar));
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
}
