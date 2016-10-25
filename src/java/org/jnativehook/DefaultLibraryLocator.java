/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
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

// Imports.
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.net.URL;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.jar.Attributes;
import java.util.jar.JarInputStream;
import java.util.jar.Manifest;
import java.util.logging.Logger;

/**
 * Default implementation of the <code>NativeLibraryLocator</code> interface.  This will first attempt to load the
 * native library from the java.library.path property.  If that fails, it will attempt to extract a library from the
 * jar based on the host operating system and architecture.
 * <p>
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.0
 * @since	2.0
 *
 * @see NativeLibraryLocator
 */
public class DefaultLibraryLocator implements NativeLibraryLocator {
	private static Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());

	/**
	 * Perform default procedures to interface with the native library. These
	 * procedures include unpacking and loading the library into the Java
	 * Virtual Machine.
	 */
	public Iterator<File> getLibraries() {
		List<File> libraries = new ArrayList<File>(1);

		String libName = System.getProperty("jnativehook.lib.name", "JNativeHook");

		// Get the package name for the GlobalScreen.
		String basePackage = GlobalScreen.class.getPackage().getName().replace('.', '/');

		// Get what the system "thinks" the library name should be.
		String libNativeName = System.mapLibraryName(libName);

		// Hack for OS X JRE 1.6 and earlier.
		libNativeName = libNativeName.replaceAll("\\.jnilib$", "\\.dylib");

		// Compile the resource path for the native library.
		StringBuilder libResourcePath = new StringBuilder("/");
		libResourcePath.append(basePackage).append("/lib/");
		libResourcePath.append(NativeSystem.getFamily().toString().toLowerCase()).append('/');
		libResourcePath.append(NativeSystem.getArchitecture().toString().toLowerCase()).append('/');
		libResourcePath.append(libNativeName);

		// Slice up the library name.
		int i = libNativeName.lastIndexOf('.');
		String libNativePrefix = libNativeName.substring(0, i) + '-';
		String libNativeSuffix = libNativeName.substring(i);
		String libNativeVersion = null;

		// This may return null in some circumstances.
		InputStream libInputStream = GlobalScreen.class.getResourceAsStream(libResourcePath.toString());
		if (libInputStream != null) {
			try {
				// Try and get version info by loading the Jar manifest as a resource stream.
				URL jarFile = GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation();
				JarInputStream jarInputStream = new JarInputStream(jarFile.openStream());

				// Try and extract a version string from the Manifest.
				Manifest manifest = jarInputStream.getManifest();
				if (manifest != null) {
					Attributes attributes = manifest.getAttributes(basePackage);

					if (attributes != null) {
						String version = attributes.getValue("Specification-Version");
						String revision = attributes.getValue("Implementation-Version");

						libNativeVersion = version + '.' + revision;
					}
					else {
						logger.warning("Invalid library manifest!\n");
					}
				}
				else {
					logger.warning("Cannot find library manifest!\n");
				}

				jarInputStream.close();
			}
			catch (IOException e) {
				logger.severe(e.getMessage());
			}


			// The temp file for this instance of the library.
			File libFile = null;
			if (libNativeVersion != null) {
				// Use the library version from the manifest to create a file.
				libFile = new File(System.getProperty("java.io.tmpdir"),
						libNativePrefix + libNativeVersion + libNativeSuffix);

				if (libFile.exists()) {
					libFile.delete();
				}
			}

			if (libraries.isEmpty()) {
				try {
					if (libFile == null) {
						// If we were unable to extract a library version from the manifest, create a new tmp file.
						libFile = File.createTempFile(libNativePrefix, libNativeSuffix);
					}

					byte[] buffer = new byte[4 * 1024];
					int size;

					// Check and see if a copy of the native lib already exists.
					FileOutputStream libOutputStream = new FileOutputStream(libFile);

					// Setup a digest...
					MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
					DigestInputStream digestInputStream = new DigestInputStream(libInputStream, sha1);

					// Read from the digest stream and write to the file steam.
					while ((size = digestInputStream.read(buffer)) != -1) {
						libOutputStream.write(buffer, 0, size);
					}

					// Close all the streams.
					digestInputStream.close();
					libInputStream.close();
					libOutputStream.close();

					// Convert the digest from byte[] to hex string.
					String sha1Sum = new BigInteger(1, sha1.digest()).toString(16).toUpperCase();
					if (libNativeVersion == null) {
						// Use the sha1 sum as a version finger print.
						libNativeVersion = sha1Sum;

						// Better late than never.
						File newFile = new File(System.getProperty("java.io.tmpdir"),
								libNativePrefix + libNativeVersion + libNativeSuffix);
						if (libFile.renameTo(newFile)) {
							libFile = newFile;
						}
					}

					// Set the library version property.
					System.setProperty("jnativehook.lib.version", libNativeVersion);

					// Add the native library to the list.
					libraries.add(libFile);

					// Log the file path and checksum.
					logger.info("Library extracted successfully: " + libFile.getPath() + " (0x" + sha1Sum + ").\n");
				}
				catch (IOException e) {
					throw new IllegalStateException(e.getMessage(), e);
				}
				catch (NoSuchAlgorithmException e) {
					throw new IllegalStateException(e.getMessage(), e);
				}
			}
		}
		else {
			logger.severe("Unable to extract the native library " + libResourcePath.toString() + "!\n");
		}

		return libraries.iterator();
	}
}
