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
import com.github.kwhat.jnativehook.DefaultLibraryLocator;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.Comparator;
import java.util.stream.Stream;

/** Runs from a JAR containing a harmless text fixture as the native resource. */
public class LibraryLocatorTest {
    public static void main(String[] args) throws Exception {
        Path directory = Files.createTempDirectory("jnativehook-locator-test");
        System.setProperty("jnativehook.lib.name", "LocatorFixture");
        System.setProperty("jnativehook.lib.path", directory.toString());
        try {
            File library = new DefaultLibraryLocator().getLibraries().next();
            check(new String(Files.readAllBytes(library.toPath()), StandardCharsets.UTF_8).equals("fixture"));
            Files.write(library.toPath(), "existing".getBytes(StandardCharsets.UTF_8));
            new DefaultLibraryLocator().getLibraries().next();
            check(new String(Files.readAllBytes(library.toPath()), StandardCharsets.UTF_8).equals("existing"));
            System.setProperty("jnativehook.lib.name", "MissingFixture");
            try {
                new DefaultLibraryLocator().getLibraries();
                throw new AssertionError("Missing resource accepted");
            } catch (RuntimeException expected) { }
            try (Stream<Path> files = Files.list(directory)) {
                check(files.count() == 1);
            }
        } finally {
            try (Stream<Path> files = Files.walk(directory)) {
                for (Path file : (Iterable<Path>) files.sorted(Comparator.reverseOrder())::iterator) {
                    Files.deleteIfExists(file);
                }
            }
        }
        System.out.println("Library extraction regressions passed");
    }
    private static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError();
        }
    }
}
