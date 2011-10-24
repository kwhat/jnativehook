/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
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
		
		/** The x86 architecture. */
		x86,
		
		/** The amd64 architecture. */
		x86_64,
		
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
		
		if (osName.toLowerCase().startsWith("windows")) {
			family = Family.WINDOWS;
		}
		else if (osName.toLowerCase().equals("linux")) {
			family = Family.LINUX;
		}
		else if (osName.toLowerCase().equals("freebsd")) {
			family = Family.FREEBSD;
		}
		else if (osName.toLowerCase().equals("mac os x")) {
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
			arch = Arch.x86;
		}
		else if (osArch.equalsIgnoreCase("x86_64") || 
				osArch.equalsIgnoreCase("amd64") || 
				osArch.equalsIgnoreCase("k8")
		) {
			arch = Arch.x86_64;
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
