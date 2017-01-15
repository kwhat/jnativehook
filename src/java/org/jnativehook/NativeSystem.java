/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
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

/**
 * A small class to determine the native system's operating system family and
 * architecture. The class is only used to determine which native library to
 * unpack and load at runtime. This class is never used if the native library
 * is load using the <code>java.library.path</code> property.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.1
 */
public class NativeSystem {

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

		/** The Apple OS X operating system family. */
		OSX,

		/** The Solaris operating system family. */
		SOLARIS,

		/** The Linux operating system family. */
		LINUX,

		/** The Windows operating system family. */
		WINDOWS,

		/** Any unsupported operating system family. */
		UNSUPPORTED
	}

	/**
	 * The system architecture enum.
	 *
	 * @see NativeSystem
	 */
	public enum Arch {
		/** The alpha architecture. */
		ALPHA,

		/** The arm architecture. */
		ARM,

		/** The itanium64 32-bit architecture. */
		IA64_32,

		/** The itanium64 architecture. */
		IA64,

		/** The mips architecture. */
		MIPS,

		/** The sparc architecture. */
		SPARC,

		/** The sparc64 architecture. */
		SPARC64,

		/** The ppc architecture. */
		PPC,

		/** The ppc64 architecture. */
		PPC64,

		/** The x86 architecture. */
		x86,

		/** The amd64 architecture. */
		x86_64,

		/** Any unsupported system architecture. */
		UNSUPPORTED
	}

	/**
	 * Determines the current operating system family.
	 *
	 * @return The current operating system family enum item.
	 */
	public static Family getFamily() {
		String osName = System.getProperty("os.name");
		Family family;

		if (osName.equalsIgnoreCase("freebsd")) {
			family = Family.FREEBSD;
		}
		else if (osName.equalsIgnoreCase("openbsd")) {
			family = Family.OPENBSD;
		}
		else if (osName.equalsIgnoreCase("mac os x")) {
			family = Family.OSX;
		}
		else if (osName.equalsIgnoreCase("solaris") ||
				osName.equalsIgnoreCase("sunos")) {
			family = Family.SOLARIS;
		}
		else if (osName.equalsIgnoreCase("linux")) {
			family = Family.LINUX;
		}
		else if (osName.toLowerCase().startsWith("windows")) {
			family = Family.WINDOWS;
		}
		else {
			family = Family.UNSUPPORTED;
		}

		return family;
	}

	/**
	 * Determines the current system architecture.
	 *
	 * @return The current system architecture.
	 */
	public static Arch getArchitecture() {
		String osArch = System.getProperty("os.arch");
		Arch arch;

		if (osArch.equalsIgnoreCase("alpha")) {
			arch = Arch.ALPHA;
		}
		else if (osArch.toLowerCase().startsWith("arm")) {
			arch = Arch.ARM;
		}
		else if (osArch.equalsIgnoreCase("ia64_32")) {
			arch = Arch.IA64_32;
		}
		else if (osArch.equalsIgnoreCase("ia64")) {
			arch = Arch.IA64;
		}
		else if (osArch.equalsIgnoreCase("mips")) {
			arch = Arch.MIPS;
		}
		else if (osArch.equalsIgnoreCase("sparc")) {
			arch = Arch.SPARC;
		}
		else if (osArch.equalsIgnoreCase("sparc64")) {
			arch = Arch.SPARC64;
		}
		else if (osArch.equalsIgnoreCase("ppc") ||
				osArch.equalsIgnoreCase("powerpc")) {
			arch = Arch.PPC;
		}
		else if (osArch.equalsIgnoreCase("ppc64") ||
				osArch.equalsIgnoreCase("powerpc64")) {
			arch = Arch.PPC64;
		}
		else if (osArch.equalsIgnoreCase("x86") ||
			osArch.equalsIgnoreCase("i386") ||
			osArch.equalsIgnoreCase("i486") ||
			osArch.equalsIgnoreCase("i586") ||
			osArch.equalsIgnoreCase("i686")) {
			arch = Arch.x86;
		}
		else if (osArch.equalsIgnoreCase("x86_64") ||
				osArch.equalsIgnoreCase("amd64") ||
				osArch.equalsIgnoreCase("k8")) {
			arch = Arch.x86_64;
		}

		else {
			arch = Arch.UNSUPPORTED;
		}

		return arch;
	}
}
