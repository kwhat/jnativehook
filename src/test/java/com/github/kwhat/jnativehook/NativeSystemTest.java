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

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.parallel.ResourceLock;
import org.junit.jupiter.api.parallel.Resources;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import static org.junit.jupiter.api.Assertions.assertEquals;

@ResourceLock(Resources.SYSTEM_PROPERTIES)
public class NativeSystemTest {
    private String originalName;
    private String originalArch;

    @BeforeEach
    void saveProperties() {
        originalName = System.getProperty("os.name");
        originalArch = System.getProperty("os.arch");
    }

    @AfterEach
    void restoreProperties() {
        restore("os.name", originalName);
        restore("os.arch", originalArch);
    }

    private static void restore(String name, String value) {
        if (value == null) {
            System.clearProperty(name);
        } else {
            System.setProperty(name, value);
        }
    }

    @ParameterizedTest
    @CsvSource({
        "FreeBSD, FREEBSD", "OpenBSD, OPENBSD", "Mac OS X, DARWIN",
        "SunOS, SOLARIS", "Solaris, SOLARIS", "Linux, LINUX", "LINUX, LINUX",
        "Windows XP, WINDOWS", "Windows 2000, WINDOWS", "Windows 2003, WINDOWS",
        "Windows 2008, WINDOWS", "Windows 2008 R2, WINDOWS", "Windows 2012, WINDOWS",
        "Windows 2012 R2, WINDOWS", "Windows 2016, WINDOWS", "Windows 2019, WINDOWS",
        "Windows 7, WINDOWS", "Windows 8, WINDOWS", "Windows 8.1, WINDOWS",
        "Windows 10, WINDOWS", "Windows 11, WINDOWS", "Something Else, UNSUPPORTED"
    })
    void detectsFamily(String name, NativeSystem.Family expected) {
        System.setProperty("os.name", name);
        assertEquals(expected, NativeSystem.getFamily());
    }

    @ParameterizedTest
    @CsvSource({
        "arm, ARM", "arm7a, ARM", "armv7l, ARM", "arm64, ARM64", "ARM64, ARM64", "aarch64, ARM64",
        "sparc, SPARC", "sparc64, SPARC64", "ppc, PPC", "powerpc, PPC",
        "ppc64, PPC64", "powerpc64, PPC64", "ppc64le, PPC64", "powerpc64le, PPC64",
        "x86, x86", "i386, x86", "i486, x86", "i586, x86", "i686, x86",
        "x86_64, x86_64", "amd64, x86_64", "k8, x86_64", "Something Else, UNSUPPORTED"
    })
    void detectsArchitecture(String name, NativeSystem.Arch expected) {
        System.setProperty("os.arch", name);
        assertEquals(expected, NativeSystem.getArchitecture());
    }
}
