import com.github.kwhat.jnativehook.*;
import com.github.kwhat.jnativehook.dispatcher.VoidDispatchService;
import com.github.kwhat.jnativehook.mouse.*;
import java.util.concurrent.*;

/** Lifecycle regressions run in a fresh VM against the test backend. */
public class LifecycleTest extends GlobalScreen {
    private static native void setRunStatus(int status);
    private static native void requestHookEvent();

    public static void main(String[] args) throws Exception {
        ExecutorService first = startAndGetDispatcher();
        registerNativeHook();
        check(first == eventExecutor && !first.isShutdown(), "idempotent registration");
        unregisterNativeHook();
        check(first.isShutdown(), "owned dispatcher shutdown");
        ExecutorService restarted = startAndGetDispatcher();
        check(restarted != first && !restarted.isShutdown(), "default dispatcher recreated");
        unregisterNativeHook();

        VoidDispatchService custom = new VoidDispatchService();
        setEventDispatcher(custom);
        registerNativeHook();
        check(!custom.isShutdown(), "registration preserves custom dispatcher");
        setEventDispatcher(custom);
        check(!custom.isShutdown(), "same dispatcher assignment");
        unregisterNativeHook();
        check(!custom.isShutdown(), "caller retains dispatcher ownership");
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
        System.out.println("Lifecycle regressions passed");
    }
    private static ExecutorService startAndGetDispatcher() throws NativeHookException {
        registerNativeHook();
        check(isNativeHookRegistered(), "hook enabled");
        return eventExecutor;
    }
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }
}
