/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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
package org.jnativehook.dispatcher;

// Imports.
import javax.swing.SwingUtilities;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Swing compatible implementation of the <code>ExecutorService</code> used to dispatch native events.  This wraps
 * event dispatching with {@link java.awt.EventQueue#invokeLater}.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.0
 * @since	2.0
 *
 * @see  java.util.concurrent.ExecutorService
 * @see  org.jnativehook.GlobalScreen#setEventDispatcher
 */
public class SwingDispatchService extends AbstractExecutorService {
	private boolean running = false;

	public SwingDispatchService() {
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

	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
		return true;
	}

	public void execute(Runnable r) {
		SwingUtilities.invokeLater(r);
	}
}
