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
     * Creates a native-system information helper.
     */
    public NativeSystem() {
        super();
    }

    /**
     * The operating system family enum.
     *
     * @see NativeSystem
     */
    public enum Family {
        /** The FreeBSD operating system family. */
        FREEBSD,
        /** The OpenBSD operating system family. */
        OPENBSD,
        /** The macOS operating system family. */
        DARWIN,
        /** The Solaris operating system family. */
        SOLARIS,
        /** The Linux operating system family. */
        LINUX,
        /** The Windows operating system family. */
        WINDOWS,

        /** Unrecognized platform or architecture. */
        UNSUPPORTED;

        @Override
        public String toString() {
            return super.toString().toLowerCase(Locale.ROOT);
        }
    }

    /**
     * The system architecture enum.
     *
     * @see NativeSystem
     */
    public enum Arch {
        /** The 32-bit ARM architecture. */
        ARM,
        /** The 64-bit ARM architecture. */
        ARM64,
        /** The 32-bit SPARC architecture. */
        SPARC,
        /** The 64-bit SPARC architecture. */
        SPARC64,
        /** The 32-bit PowerPC architecture. */
        PPC,
        /** The 64-bit PowerPC architecture. */
        PPC64,
        /** The 32-bit x86 architecture. */
        x86,
        /** The 64-bit x86 architecture. */
        x86_64,

        /** Unrecognized platform or architecture. */
        UNSUPPORTED;

        @Override
        public String toString() {
            return super.toString().toLowerCase(Locale.ROOT);
        }
    }

    /**
     * Determines the current operating system family.
     *
     * @return The current operating system family enum item.
     */
    public static Family getFamily() {
        final String osName = System.getProperty("os.name").toLowerCase(Locale.ROOT);
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
        final String osArch = System.getProperty("os.arch").toLowerCase(Locale.ROOT);
        Arch arch = Arch.UNSUPPORTED;

        if (osArch.startsWith("arm")) {
            arch = Arch.ARM;
        } else if (osArch.equals("aarch64")) {
            arch = Arch.ARM64;
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
