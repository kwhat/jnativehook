/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
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
    private boolean running = false;

    public VoidDispatchService() {
        running = true;
    }

    public void shutdown() {
        running = false;
    }

    public List<Runnable> shutdownNow() {
        running = false;
        return new ArrayList<Runnable>(0);
    }

    public boolean isShutdown() {
        return !running;
    }

    public boolean isTerminated() {
        return !running;
    }

    public boolean awaitTermination(long timeout, TimeUnit unit) {
        return true;
    }

    public void execute(Runnable r) {
        r.run();
    }
}
