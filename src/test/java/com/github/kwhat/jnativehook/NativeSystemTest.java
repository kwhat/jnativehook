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
import static org.junit.jupiter.api.Assertions.assertEquals;

public class NativeSystemTest {
	@Test
	public void testGetFamily() {
	    System.setProperty("os.name", "FreeBSD");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.FREEBSD);

        System.setProperty("os.name", "OpenBSD");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.OPENBSD);

        System.setProperty("os.name", "Mac OS X");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.DARWIN);

        System.setProperty("os.name", "SunOS");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.SOLARIS);

        System.setProperty("os.name", "Solaris");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.SOLARIS);

        System.setProperty("os.name", "Linux");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.LINUX);

        System.setProperty("os.name", "Windows 2000");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2003");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2008");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2008 R2");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2012");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2012 R2");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2016");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 2019");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows XP");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 7");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 8");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Windows 8.1");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.WINDOWS);

        System.setProperty("os.name", "Something Else");
        assertEquals(NativeSystem.getFamily(), NativeSystem.Family.UNSUPPORTED);
	}

	@Test
	public void testGetArchitecture() {
	    System.setProperty("os.arch", "arm");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.ARM);

        System.setProperty("os.arch", "arm7a");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.ARM);

        System.setProperty("os.arch", "aarch64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.ARM64);

        System.setProperty("os.arch", "sparc");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.SPARC);

        System.setProperty("os.arch", "sparc64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.SPARC64);

        System.setProperty("os.arch", "ppc");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.PPC);

        System.setProperty("os.arch", "powerpc");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.PPC);

        System.setProperty("os.arch", "ppc64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.PPC64);

        System.setProperty("os.arch", "powerpc64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.PPC64);

        System.setProperty("os.arch", "x86");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86);

        System.setProperty("os.arch", "i386");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86);

        System.setProperty("os.arch", "i486");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86);

        System.setProperty("os.arch", "i586");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86);

        System.setProperty("os.arch", "i686");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86);

        System.setProperty("os.arch", "x86_64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86_64);

        System.setProperty("os.arch", "amd64");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86_64);

        System.setProperty("os.arch", "k8");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.x86_64);

        System.setProperty("os.arch", "Something Else");
        assertEquals(NativeSystem.getArchitecture(), NativeSystem.Arch.UNSUPPORTED);
	}
}
