package org.jnativehook;

public class OperatingSystem {
	public enum Family {
		Windows,
		Linux,
		FreeBSD,
		OpenBSD,
		OSX,
		unsupported
	}
	
	public enum Arch {
		i586,
		amd64,
		ppc,
		ppc64,
		arm,
		mips,
		sparc,
		alpha,
		unsupported
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
			family = Family.Windows;
		}
		else if (os_name.equalsIgnoreCase("Linux")) {
			family = Family.Linux;
		}
		else if (os_name.equalsIgnoreCase("FreeBSD")) {
			family = Family.FreeBSD;
		}
		else if (os_name.equalsIgnoreCase("OpenBSD")) {
			family = Family.OpenBSD;
		}
		else if (os_name.equalsIgnoreCase("Mac OS X")) {
			family = Family.OSX;
		}
		else {
			family = Family.unsupported;
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
			arch = Arch.i586;
		}
		else if (os_arch.equalsIgnoreCase("x86_64") || 
				os_arch.equalsIgnoreCase("amd64") || 
				os_arch.equalsIgnoreCase("k8")
		) {
			arch = Arch.amd64;
		}
		else if (os_arch.equalsIgnoreCase("ppc") ||
				os_arch.equalsIgnoreCase("PowerPC")
		) {
			arch = Arch.ppc;
		}
		else if (os_arch.equalsIgnoreCase("ppc64")) {
			arch = Arch.ppc64;
		}
		else {
			arch = Arch.unsupported;
		}
		
		return arch;
	}
}
