package com.github.kwhat.jnativehook.dispatcher;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.*;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;
import static org.junit.jupiter.api.Assertions.*;

@Timeout(15)
class DefaultDispatchServiceTest {
    @Test
    void deliversAcceptedEventsInOrderAndDrainsOnShutdown() throws Exception {
        DefaultDispatchService service = new DefaultDispatchService();
        List<Integer> delivered = Collections.synchronizedList(new ArrayList<>());
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        Future<?> first = service.submit(() -> {
            entered.countDown();
            try {
                if (!release.await(5, TimeUnit.SECONDS)) throw new AssertionError("Task not released");
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new AssertionError(e);
            }
            delivered.add(1);
        });
        try {
            assertTrue(entered.await(5, TimeUnit.SECONDS));
            Future<?> second = service.submit(() -> delivered.add(2));
            Future<?> third = service.submit(() -> delivered.add(3));
            service.shutdown();
            assertThrows(RejectedExecutionException.class, () -> service.execute(() -> { }));
            release.countDown();
            first.get(5, TimeUnit.SECONDS);
            second.get(5, TimeUnit.SECONDS);
            third.get(5, TimeUnit.SECONDS);
            assertTrue(service.awaitTermination(5, TimeUnit.SECONDS));
            assertEquals(Arrays.asList(1, 2, 3), delivered);
        } finally {
            release.countDown();
            service.shutdownNow();
        }
    }

    @Test
    void dispatchesOffTheSubmittingThreadAndSurvivesSubmittedTaskFailure() throws Exception {
        DefaultDispatchService service = new DefaultDispatchService();
        try {
            Thread caller = Thread.currentThread();
            assertNotSame(caller, service.submit(Thread::currentThread).get(5, TimeUnit.SECONDS));
            IllegalStateException failure = new IllegalStateException("listener failed");
            Future<?> failed = service.submit(() -> { throw failure; });
            assertSame(failure, assertThrows(ExecutionException.class,
                () -> failed.get(5, TimeUnit.SECONDS)).getCause());
            assertEquals("next event", service.submit(() -> "next event").get(5, TimeUnit.SECONDS));
        } finally {
            service.shutdownNow();
        }
    }
}
