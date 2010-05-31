/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook;

/**
 * A small class to determine the native system's operating system family and 
 * architecture. The class is only used to determine which library to unpack 
 * and load at runtime. 
 */
public class NativeSystem {
	
	/**
	 * The operating system family enum.
	 */
	public enum Family {
		
		/** The Windows operating system family. */
		WINDOWS,
		
		/** The Linux operating system family. */
		LINUX,
		
		/** The FreeBSD operating system family. */
		FREEBSD,
		
		/** The OpenBSD operating system family. */
		OPENBSD,
		
		/** The Apple OS X operating system family. */
		OSX,
		
		/** Any unsupported operating system family. */
		UNSUPPORTED
	}
	
	/**
	 * The system architecture enum.
	 */
	public enum Arch {
		
		/** The i586 architecture. */
		I586,
		
		/** The amd64 architecture. */
		AMD64,
		
		/** The ppc architecture. */
		PPC,
		
		/** The ppc64 architecture. */
		PPC64,
		
		/** The arm architecture. */
		ARM,
		
		/** The mips architecture. */
		MIPS,
		
		/** The sparc architecture. */
		SPARC,
		
		/** The alpha architecture. */
		ALPHA,
		
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
		
		if (osName.equalsIgnoreCase("Windows NT") || 
			osName.equalsIgnoreCase("Windows 2000") || 
			osName.equalsIgnoreCase("Windows XP") ||
			osName.equalsIgnoreCase("Windows 2003") ||
			osName.equalsIgnoreCase("Windows Vista") ||
			osName.equalsIgnoreCase("Windows 7")
		) {
			family = Family.WINDOWS;
		}
		else if (osName.equalsIgnoreCase("Linux")) {
			family = Family.LINUX;
		}
		else if (osName.equalsIgnoreCase("FreeBSD")) {
			family = Family.FREEBSD;
		}
		else if (osName.equalsIgnoreCase("Mac OS X")) {
			family = Family.OSX;
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
		
		if (osArch.equalsIgnoreCase("x86") || 
			osArch.equalsIgnoreCase("i386") || 
			osArch.equalsIgnoreCase("i486") ||
			osArch.equalsIgnoreCase("i586") ||
			osArch.equalsIgnoreCase("i686")
		) {
			arch = Arch.I586;
		}
		else if (osArch.equalsIgnoreCase("x86_64") || 
				osArch.equalsIgnoreCase("amd64") || 
				osArch.equalsIgnoreCase("k8")
		) {
			arch = Arch.AMD64;
		}
		else if (osArch.equalsIgnoreCase("ppc") ||
				osArch.equalsIgnoreCase("PowerPC")
		) {
			arch = Arch.PPC;
		}
		else if (osArch.equalsIgnoreCase("ppc64")) {
			arch = Arch.PPC64;
		}
		else {
			arch = Arch.UNSUPPORTED;
		}
		
		return arch;
	}
}
