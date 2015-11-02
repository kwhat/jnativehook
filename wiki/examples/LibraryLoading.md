### Advanced Library Loading
As of version 2.0, JNativeHook includes a native library loading interface called [NativeLibraryLocator](../../src/java/org/jnativehook/NativeLibraryLocator.java).
You can implement this interface to customize what libraries are loaded, as well as where they are loaded from. 

```java
public class UrlLibraryLocator implements NativeLibraryLocator {
	public Iterator<File> getLibraries() {
		String libName = "MyLibrary";
		
		// Get what the system "thinks" the library name should be.
		String libNativeName = System.mapLibraryName(libName);
		
		// Hack for OS X JRE 1.6 and earlier.
		libNativeName = libNativeName.replaceAll("\\.jnilib$", "\\.dylib");
		
		String libExt = libNativeName.substring(libNativeName.lastIndexOf('.'));
		
		URL libUrl = new URL("http://www.mydomain.told/pathToFile/" + libNativeName);
		File libFile = File.createTempFile(tempFile, libExt);
		
		libFile.deleteOnExit(); 
		
		FileUtils.copyURLToFile(libUrl, libFile);
		
		ArrayList<libFile> list = new ArrayList(1);
		list.add(file);
		
		return list;
	}
}
```
