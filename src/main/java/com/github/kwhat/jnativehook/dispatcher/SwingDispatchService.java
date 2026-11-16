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
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.TimeUnit;
import javax.swing.SwingUtilities;

/**
 * Swing compatible implementation of the <code>ExecutorService</code> used to dispatch native
 * events.  This wraps event dispatching with {@link java.awt.EventQueue#invokeLater}.
 *
 * @author Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version 2.0
 * @since 2.0
 * @see java.util.concurrent.ExecutorService
 * @see GlobalScreen#setEventDispatcher
 */
public class SwingDispatchService extends AbstractExecutorService {
    private boolean running = true;
    private int active;
    private final Map<Runnable, Runnable> pending = new LinkedHashMap<>();

    /** Creates a Swing event dispatcher. */
    public SwingDispatchService() {
        running = true;
    }

    public synchronized void shutdown() {
        running = false;
        notifyAll();
    }

    public synchronized List<Runnable> shutdownNow() {
        shutdown();
        List<Runnable> cancelled = new ArrayList<>(pending.values());
        pending.clear();
        return cancelled;
    }

    public synchronized boolean isShutdown() {
        return !running;
    }

    public synchronized boolean isTerminated() {
        return !running && active == 0 && pending.isEmpty();
    }

    public synchronized boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        long remaining = unit.toNanos(timeout);
        long last = System.nanoTime();
        while (!isTerminated()) {
            if (remaining <= 0) {
                return false;
            }
            TimeUnit.NANOSECONDS.timedWait(this, remaining);
            long now = System.nanoTime();
            remaining -= now - last;
            last = now;
        }
        return true;
    }

    public void execute(Runnable command) {
        Objects.requireNonNull(command, "command");
        Runnable task = new Runnable() {
            public void run() {
                synchronized (SwingDispatchService.this) {
                    if (pending.remove(this) == null) {
                        return;
                    }
                    active++;
                }
                try {
                    command.run();
                } finally {
                    synchronized (SwingDispatchService.this) {
                        active--;
                        SwingDispatchService.this.notifyAll();
                    }
                }
            }
        };
        synchronized (this) {
            if (!running) {
                throw new RejectedExecutionException("Dispatcher is shut down.");
            }
            pending.put(task, command);
            try {
                SwingUtilities.invokeLater(task);
            } catch (RuntimeException | Error error) {
                pending.remove(task);
                notifyAll();
                throw error;
            }
        }
    }
}
