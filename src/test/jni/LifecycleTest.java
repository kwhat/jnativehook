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
import com.github.kwhat.jnativehook.*;
import com.github.kwhat.jnativehook.dispatcher.VoidDispatchService;
import com.github.kwhat.jnativehook.mouse.*;
import java.util.concurrent.*;

/** Lifecycle regressions run in a fresh VM against the test backend. */
public class LifecycleTest extends GlobalScreen {
    private static native void setRunStatus(int status);
    private static native void requestHookEvent();

    public static void main(String[] args) throws Exception {
        ExecutorService initial = eventExecutor;
        check(initial instanceof com.github.kwhat.jnativehook.dispatcher.DefaultDispatchService,
                "default dispatcher initialized before registration");
        ExecutorService first = startAndGetDispatcher();
        check(first == initial, "registration uses the unconfigured default");
        registerNativeHook();
        check(first == eventExecutor && !first.isShutdown(), "idempotent registration");
        unregisterNativeHook();
        check(!first.isShutdown(), "hook shutdown preserves selected dispatcher");
        ExecutorService restarted = startAndGetDispatcher();
        check(restarted == first && !restarted.isShutdown(), "restart retains default dispatcher");
        unregisterNativeHook();

        VoidDispatchService custom = new VoidDispatchService();
        setEventDispatcher(custom);
        check(first.isShutdown(), "replacement shuts down default dispatcher");
        registerNativeHook();
        check(!custom.isShutdown(), "registration preserves custom dispatcher");
        setEventDispatcher(custom);
        check(!custom.isShutdown(), "same dispatcher assignment");
        unregisterNativeHook();
        check(!custom.isShutdown(), "hook shutdown preserves custom dispatcher");
        registerNativeHook();
        CountDownLatch called = new CountDownLatch(1);
        NativeMouseMotionListener listener = new NativeMouseMotionListener() {
            public void nativeMouseMoved(NativeMouseEvent event) {
                try {
                    unregisterNativeHook();
                    called.countDown();
                } catch (NativeHookException error) {
                    throw new AssertionError(error);
                }
            }
        };
        addNativeMouseMotionListener(listener);
        requestHookEvent();
        check(called.await(5, TimeUnit.SECONDS), "unregister from a synchronous listener");
        hookThread.join(5000);
        check(!hookThread.isAlive(), "hook thread stopped");
        removeNativeMouseMotionListener(listener);
        setEventDispatcher(null);
        check(custom.isShutdown(), "replacement shuts down custom dispatcher");
        registerNativeHook();
        check(eventExecutor == null, "explicit null dispatcher preserved");
        unregisterNativeHook();
        setRunStatus(NativeHookException.EPOLL_CREATE);
        try {
            registerNativeHook();
            throw new AssertionError("Expected startup failure");
        } catch (NativeHookException expected) {
            check(expected.getCode() == NativeHookException.EPOLL_CREATE, "startup error preserved");
        }
        hookThread.join(5000);
        check(!hookThread.isAlive() && !isNativeHookRegistered(), "failed hook stopped");
        setRunStatus(0);
        registerNativeHook();
        unregisterNativeHook();
        CountingDispatcher stopped = new CountingDispatcher(false);
        setEventDispatcher(stopped);
        stopped.shutdown();
        registerNativeHook();
        check(eventExecutor == stopped, "registration retains a stopped selected dispatcher");
        HookThreadAccess.deliver(new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 0, 0, 0));
        check(stopped.submissions == 0, "no jobs submitted to a stopped dispatcher");
        unregisterNativeHook();

        CountingDispatcher racing = new CountingDispatcher(true);
        setEventDispatcher(racing);
        HookThreadAccess.deliver(new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 0, 0, 0));
        check(racing.submissions == 1 && racing.isShutdown(), "shutdown during submission is tolerated");
        setEventDispatcher(null);
        System.out.println("Lifecycle regressions passed");
    }
    private static class HookThreadAccess extends NativeHookThread {
        private static void deliver(NativeInputEvent event) {
            dispatchEvent(event);
        }
    }

    private static class CountingDispatcher extends VoidDispatchService {
        private final boolean stopOnSubmit;
        private int submissions;

        CountingDispatcher(boolean stopOnSubmit) {
            this.stopOnSubmit = stopOnSubmit;
        }

        @Override
        public void execute(Runnable command) {
            submissions++;
            if (stopOnSubmit) {
                shutdown();
            }
            super.execute(command);
        }
    }

    private static ExecutorService startAndGetDispatcher() throws NativeHookException {
        registerNativeHook();
        check(isNativeHookRegistered(), "hook enabled");
        return eventExecutor;
    }
    private static void check(boolean condition, String message) {
        if (!condition) {
            throw new AssertionError(message);
        }
    }
}
