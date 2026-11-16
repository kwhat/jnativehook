package com.github.kwhat.jnativehook.dispatcher;

import java.util.Arrays;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;
import javax.swing.SwingUtilities;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;
import static org.junit.jupiter.api.Assertions.*;

@Timeout(15)
class SwingDispatchServiceTest {
    @Test
    void executesOnEdtAndDrainsAcceptedTasksAfterShutdown() throws Exception {
        SwingDispatchService service = new SwingDispatchService();
        try {
            Future<Boolean> onEdt = service.submit(SwingUtilities::isEventDispatchThread);
            service.shutdown();
            assertTrue(onEdt.get(5, TimeUnit.SECONDS));
            assertTrue(service.awaitTermination(5, TimeUnit.SECONDS));
            assertThrows(RejectedExecutionException.class, () -> service.execute(() -> { }));
        } finally {
            service.shutdownNow();
        }
    }

    @Test
    void shutdownNowReturnsAndCancelsPendingCommandsInOrder() throws Exception {
        SwingDispatchService service = new SwingDispatchService();
        AtomicInteger executed = new AtomicInteger();
        Runnable first = executed::incrementAndGet;
        Runnable second = executed::incrementAndGet;
        try (EdtBlocker blocker = new EdtBlocker()) {
            service.execute(first);
            service.execute(second);
            service.execute(first);
            service.shutdown();
            assertFalse(service.isTerminated());
            assertFalse(service.awaitTermination(0, TimeUnit.SECONDS));
            assertEquals(Arrays.asList(first, second, first), service.shutdownNow());
            assertTrue(service.isTerminated());
            assertTrue(service.shutdownNow().isEmpty());
        } finally {
            service.shutdownNow();
        }
        SwingUtilities.invokeAndWait(() -> { });
        assertEquals(0, executed.get());
    }

    @Test
    void inFlightWorkPreventsPrematureTermination() throws Exception {
        SwingDispatchService service = new SwingDispatchService();
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        Future<?> work = service.submit(() -> {
            entered.countDown();
            await(release);
        });
        try {
            assertTrue(entered.await(5, TimeUnit.SECONDS));
            service.shutdownNow();
            assertFalse(service.isTerminated());
            assertFalse(service.awaitTermination(0, TimeUnit.SECONDS));
        } finally {
            release.countDown();
            service.shutdownNow();
        }
        work.get(5, TimeUnit.SECONDS);
        assertTrue(service.awaitTermination(5, TimeUnit.SECONDS));
    }

    @Test
    void repeatedSubmissionOfSameCommandRunsEachSubmission() throws Exception {
        SwingDispatchService service = new SwingDispatchService();
        AtomicInteger count = new AtomicInteger();
        Runnable command = count::incrementAndGet;
        try (EdtBlocker blocker = new EdtBlocker()) {
            service.execute(command);
            service.execute(command);
            service.shutdown();
        } finally {
            service.shutdown();
        }
        assertTrue(service.awaitTermination(5, TimeUnit.SECONDS));
        assertEquals(2, count.get());
    }

    @Test
    void rejectsNullTasksWithoutQueuingWork() throws Exception {
        SwingDispatchService service = new SwingDispatchService();
        assertThrows(NullPointerException.class, () -> service.execute(null));
        service.shutdown();
        assertTrue(service.awaitTermination(0, TimeUnit.SECONDS));
    }

    private static void await(CountDownLatch latch) {
        try {
            if (!latch.await(5, TimeUnit.SECONDS)) throw new AssertionError("Task was not released");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new AssertionError(e);
        }
    }

    private static final class EdtBlocker implements AutoCloseable {
        private final CountDownLatch release = new CountDownLatch(1);
        EdtBlocker() throws Exception {
            CountDownLatch entered = new CountDownLatch(1);
            SwingUtilities.invokeLater(() -> {
                entered.countDown();
                await(release);
            });
            if (!entered.await(5, TimeUnit.SECONDS)) {
                release.countDown();
                fail("EDT did not start");
            }
        }
        public void close() { release.countDown(); }
    }
}
