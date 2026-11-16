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

import com.github.kwhat.jnativehook.GlobalScreen;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.RejectedExecutionException;
import java.util.Objects;

/**
 * Void implementation of the <code>ExecutorService</code> used to dispatch native events directly on the operating
 * system's event delivery thread.  Because this queue does not hand off events to a different thread, it is important
 * that you do not block on native event callback functions as it will delay or prevent subsequent events from being
 * delivered by the operating system.  This dispatcher should only be used by those attempting to consume native events.
 *
 * @author Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version 2.2
 * @see java.util.concurrent.ExecutorService
 * @see GlobalScreen#setEventDispatcher
 * @since 2.2
 */
public class VoidDispatchService extends AbstractExecutorService {
    private boolean running = true;
    private int active;

    /**
     * Creates a event dispatcher that executes on the os delivery thread.
     */
    public VoidDispatchService() {
        running = true;
    }

    public synchronized void shutdown() {
        running = false;
        notifyAll();
    }

    public synchronized List<Runnable> shutdownNow() {
        shutdown();
        return new ArrayList<>(0);
    }

    public synchronized boolean isShutdown() {
        return !running;
    }

    public synchronized boolean isTerminated() {
        return !running && active == 0;
    }

    public synchronized boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        long remaining = unit.toNanos(timeout);
        long last = System.nanoTime();
        while (!isTerminated()) {
            if (remaining <= 0) return false;
            TimeUnit.NANOSECONDS.timedWait(this, remaining);
            long now = System.nanoTime();
            remaining -= now - last;
            last = now;
        }
        return true;
    }

    public void execute(Runnable task) {
        Objects.requireNonNull(task, "task");
        synchronized (this) {
            if (!running) throw new RejectedExecutionException("Dispatcher is shut down.");
            active++;
        }
        try {
            task.run();
        } finally {
            synchronized (this) {
                active--;
                notifyAll();
            }
        }
    }
}
