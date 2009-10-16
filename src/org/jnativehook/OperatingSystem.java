package org.jnativehook;

public class OperatingSystem {
	public enum Family {
		WINDOWS,
		LINUX,
		FREEBSD,
		OPENBSD,
		OSX,
		UNSUPPORTED
	}
	
	public enum Arch {
		I586,
		AMD64,
		PPC,
		PPC64,
		ARM,
		MIPS,
		SPARC,
		ALPHA,
		UNSUPPORTED
	}
	
	public static Family getFamily() {
		String os_name = System.getProperty("os.name");
		Family family;
		
		if (os_name.equalsIgnoreCase("Windows NT") || 
			os_name.equalsIgnoreCase("Windows 2000") || 
			os_name.equalsIgnoreCase("Windows XP") ||
			os_name.equalsIgnoreCase("Windows 2003") ||
			os_name.equalsIgnoreCase("Windows Vista") ||
			os_name.equalsIgnoreCase("Windows 7")
		) {
			family = Family.WINDOWS;
		}
		else if (os_name.equalsIgnoreCase("Linux")) {
			family = Family.LINUX;
		}
		else if (os_name.equalsIgnoreCase("FreeBSD")) {
			family = Family.FREEBSD;
		}
		else if (os_name.equalsIgnoreCase("OpenBSD")) {
			family = Family.OPENBSD;
		}
		else if (os_name.equalsIgnoreCase("Mac OS X")) {
			family = Family.OSX;
		}
		else {
			family = Family.UNSUPPORTED;
		}
		
		return family;
	}
	
	public static Arch getArchitecture() {
		String os_arch = System.getProperty("os.arch");
		Arch arch;
		
		if (os_arch.equalsIgnoreCase("x86") || 
			os_arch.equalsIgnoreCase("i386") || 
			os_arch.equalsIgnoreCase("i486") ||
			os_arch.equalsIgnoreCase("i586") ||
			os_arch.equalsIgnoreCase("i686")
		) {
			arch = Arch.I586;
		}
		else if (os_arch.equalsIgnoreCase("x86_64") || 
				os_arch.equalsIgnoreCase("amd64") || 
				os_arch.equalsIgnoreCase("k8")
		) {
			arch = Arch.AMD64;
		}
		else if (os_arch.equalsIgnoreCase("ppc") ||
				os_arch.equalsIgnoreCase("PowerPC")
		) {
			arch = Arch.PPC;
		}
		else if (os_arch.equalsIgnoreCase("ppc64")) {
			arch = Arch.PPC64;
		}
		else {
			arch = Arch.UNSUPPORTED;
		}
		
		return arch;
	}
}
