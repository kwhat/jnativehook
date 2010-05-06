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

// TODO: Auto-generated Javadoc
/**
 * The Class NativeSystem.
 */
public class NativeSystem {
	
	/**
	 * The Enum Family.
	 */
	public enum Family {
		
		/** The WINDOWS. */
		WINDOWS,
		
		/** The LINUX. */
		LINUX,
		
		/** The FREEBSD. */
		FREEBSD,
		
		/** The OPENBSD. */
		OPENBSD,
		
		/** The OSX. */
		OSX,
		
		/** The UNSUPPORTED. */
		UNSUPPORTED
	}
	
	/**
	 * The Enum Arch.
	 */
	public enum Arch {
		
		/** The I586. */
		I586,
		
		/** The AM d64. */
		AMD64,
		
		/** The PPC. */
		PPC,
		
		/** The PP c64. */
		PPC64,
		
		/** The ARM. */
		ARM,
		
		/** The MIPS. */
		MIPS,
		
		/** The SPARC. */
		SPARC,
		
		/** The ALPHA. */
		ALPHA,
		
		/** The UNSUPPORTED. */
		UNSUPPORTED
	}
	
	/**
	 * Gets the family.
	 *
	 * @return the family
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
		else if (osName.equalsIgnoreCase("OpenBSD")) {
			family = Family.OPENBSD;
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
	 * Gets the architecture.
	 *
	 * @return the architecture
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
