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
package com.github.kwhat.jnativehook.dispatcher;

import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.TimeUnit;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class VoidDispatchServiceTest {
    @Test
    public void testConstructor() {
        VoidDispatchService service = new VoidDispatchService();

        assertFalse(service.isShutdown());
        assertFalse(service.isTerminated());
    }

    @Test
    public void testShutdown() {
        VoidDispatchService service = new VoidDispatchService();
        service.shutdown();

        assertTrue(service.isShutdown());
        assertTrue(service.isTerminated());
    }

    @Test
    public void testShutdownNow() {
        VoidDispatchService service = new VoidDispatchService();
        List<Runnable> tasks = service.shutdownNow();

        assertEquals(0, tasks.size());
        assertTrue(service.isShutdown());
        assertTrue(service.isTerminated());
    }

    @Test
    public void testAwaitTermination() throws InterruptedException {
        VoidDispatchService service = new VoidDispatchService();

        assertFalse(service.awaitTermination(0L, TimeUnit.SECONDS));
        service.shutdown();
        assertTrue(service.awaitTermination(1L, TimeUnit.SECONDS));
    }

    @Test
    public void testExecute() {
        VoidDispatchService service = new VoidDispatchService();

        final boolean[] hasRun = {false};
        service.execute(new Runnable() {
            @Override
            public void run() {
                hasRun[0] = true;
            }
        });

        assertTrue(hasRun[0]);
    }

    @Test
    public void executesOnCallingThread() {
        Thread caller = Thread.currentThread();
        new VoidDispatchService().execute(() -> assertSame(caller, Thread.currentThread()));
    }

    @Test
    public void rejectsWorkAfterBothShutdownVariants() {
        VoidDispatchService graceful = new VoidDispatchService();
        graceful.shutdown();
        assertThrows(RejectedExecutionException.class, () -> graceful.execute(() -> { }));
        VoidDispatchService immediate = new VoidDispatchService();
        immediate.shutdownNow();
        assertThrows(RejectedExecutionException.class, () -> immediate.execute(() -> { }));
    }

    @Test
    public void taskFailureDoesNotPreventTermination() throws InterruptedException {
        VoidDispatchService service = new VoidDispatchService();
        IllegalStateException failure = new IllegalStateException("task failed");
        assertSame(failure, assertThrows(IllegalStateException.class, () -> service.execute(() -> {
            throw failure;
        })));
        service.shutdown();
        assertTrue(service.awaitTermination(0, TimeUnit.SECONDS));
    }

    @Test
    public void inFlightTaskMustFinishBeforeTermination() throws Exception {
        VoidDispatchService service = new VoidDispatchService();
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        FutureTask<Void> task = new FutureTask<>(() -> {
            service.execute(() -> {
                entered.countDown();
                try {
                    if (!release.await(5, TimeUnit.SECONDS)) {
                        throw new AssertionError("Task not released");
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new AssertionError(e);
                }
            });
            return null;
        });
        Thread worker = new Thread(task, "void-dispatch-test");
        worker.setDaemon(true);
        worker.start();
        try {
            assertTrue(entered.await(5, TimeUnit.SECONDS));
            assertTrue(service.shutdownNow().isEmpty());
            assertFalse(service.isTerminated());
            assertFalse(service.awaitTermination(0, TimeUnit.SECONDS));
        } finally {
            release.countDown();
            service.shutdownNow();
        }
        task.get(5, TimeUnit.SECONDS);
        assertTrue(service.awaitTermination(5, TimeUnit.SECONDS));
    }

    @Test
    public void rejectsNullTasks() throws InterruptedException {
        VoidDispatchService service = new VoidDispatchService();
        assertThrows(NullPointerException.class, () -> service.execute(null));
        service.shutdown();
        assertTrue(service.awaitTermination(0, TimeUnit.SECONDS));
    }
}
