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
        if (!condition) throw new AssertionError();
    }
}
