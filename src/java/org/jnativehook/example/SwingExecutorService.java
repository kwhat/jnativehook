/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.example;

// Imports.
import java.awt.EventQueue;
import java.awt.Toolkit;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.TimeUnit;

public class SwingExecutorService extends AbstractExecutorService {
	private EventQueue queue;
	
	public SwingExecutorService() {
		queue = Toolkit.getDefaultToolkit().getSystemEventQueue();
	}
	
	public void shutdown() {
		queue = null;
	}

	public List<Runnable> shutdownNow() {
		return new ArrayList<Runnable>(0);
	}

	public boolean isShutdown() {
		return queue == null;
	}

	public boolean isTerminated() {
		return queue == null;
	}

	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
		return true;
	}

	public void execute(Runnable r) {
		EventQueue.invokeLater(r);
	}
}
