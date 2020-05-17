/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2020 Alexander Barker.  All Rights Received.
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

import java.util.Locale;

/**
 * A small class to determine the native system's operating system family and architecture. The
 * class is only used to determine which native library to unpack and load at runtime. This class is
 * never used if the native library is loaded using the <code>java.library.path</code> property.
 *
 * @author Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version 2.2
 * @since   1.0
 */
public class NativeSystem {
    /**
     * According to the docs, Locale.ROOT is the same as the locale whose language, country, and
     * variant are empty ("") strings.  This is required to maintain Java 1.5 compatibility.
     */
    protected static final Locale ROOT_LOCALE = new Locale("", "", "");

    /**
     * The operating system family enum.
     *
     * @see NativeSystem
     */
    public enum Family {
        FREEBSD,
        OPENBSD,
        DARWIN,
        SOLARIS,
        LINUX,
        WINDOWS,

        UNSUPPORTED;

        @Override
        public String toString() {
            return super.toString().toLowerCase(NativeSystem.ROOT_LOCALE);
        }
    }

    /**
     * The system architecture enum.
     *
     * @see NativeSystem
     */
    public enum Arch {
        ARM,
        ARM64,
        SPARC,
        SPARC64,
        PPC,
        PPC64,
        x86,
        x86_64,

        UNSUPPORTED;

        @Override
        public String toString() {
            return super.toString().toLowerCase(NativeSystem.ROOT_LOCALE);
        }
    }

    /**
     * Determines the current operating system family.
     *
     * @return The current operating system family enum item.
     */
    public static Family getFamily() {
        final String osName = System.getProperty("os.name").toLowerCase(NativeSystem.ROOT_LOCALE);
        Family family = Family.UNSUPPORTED;

        if (osName.equals("freebsd")) {
            family = Family.FREEBSD;
        } else if (osName.equals("openbsd")) {
            family = Family.OPENBSD;
        } else if (osName.equals("mac os x")) {
            family = Family.DARWIN;
        } else if (osName.equals("solaris")
                || osName.equals("sunos")) {
            family = Family.SOLARIS;
        } else if (osName.equals("linux")) {
            family = Family.LINUX;
        } else if (osName.startsWith("windows")) {
            family = Family.WINDOWS;
        }

        return family;
    }

    /**
     * Determines the current system architecture.
     *
     * @return The current system architecture.
     */
    public static Arch getArchitecture() {
        final String osArch = System.getProperty("os.arch").toLowerCase(NativeSystem.ROOT_LOCALE);
        Arch arch = Arch.UNSUPPORTED;

        if (osArch.equals("arm")) {
            arch = Arch.ARM;
        } else if (osArch.equals("sparc")) {
            arch = Arch.SPARC;
        } else if (osArch.equals("sparc64")) {
            arch = Arch.SPARC64;
        } else if (osArch.equals("ppc")
                || osArch.equals("powerpc")) {
            arch = Arch.PPC;
        } else if (osArch.equals("ppc64")
                || osArch.equals("powerpc64")) {
            arch = Arch.PPC64;
        } else if (osArch.equals("x86")
                || osArch.equals("i386")
                || osArch.equals("i486")
                || osArch.equals("i586")
                || osArch.equals("i686")) {
            arch = Arch.x86;
        } else if (osArch.equals("x86_64")
                || osArch.equals("amd64")
                || osArch.equals("k8")) {
            arch = Arch.x86_64;
        }

        return arch;
    }
}
