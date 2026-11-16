import com.github.kwhat.jnativehook.*;
import com.github.kwhat.jnativehook.dispatcher.VoidDispatchService;
import com.github.kwhat.jnativehook.keyboard.*;
import com.github.kwhat.jnativehook.mouse.*;
import java.awt.event.MouseWheelEvent;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.logging.*;

/** Exercises the JNI bridge against api_stub.c without accessing system input. */
public class ApiSmokeTest {
    private static final AtomicInteger delivered = new AtomicInteger();
    private static native void emitFromNativeThread();
    private static native void emitTypedScalar();
    public static void main(String[] args) {
        GlobalScreen.setEventDispatcher(new VoidDispatchService());
        GlobalScreen.addNativeKeyListener(new NativeKeyListener() {
            public void nativeKeyPressed(NativeKeyEvent event) {
                check(event.getKeyCode() == NativeKeyEvent.VC_CONTROL);
                check(event.getKeyLocation() == NativeKeyEvent.KEY_LOCATION_RIGHT);
                check(event.isEmulated());
                event.consume();
                check(event.isConsumed());
                delivered.incrementAndGet();
            }
        });
        GlobalScreen.addNativeMouseWheelListener(new NativeMouseWheelListener() {
            public void nativeMouseWheelMoved(NativeMouseWheelEvent event) {
            check(event.getPreciseWheelRotation() == 0.25);
            check(event.getWheelDirection() == NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION);
            check(event.getScrollAmount() == 1);
            check(event.getX() == -12);
            delivered.incrementAndGet();
            }
        });
        GlobalScreen.addNativeMouseMotionListener(new NativeMouseMotionListener() {
            public void nativeMouseMoved(NativeMouseEvent event) {
                check(event.getDeltaX() == -3 && event.getDeltaY() == 7);
                delivered.incrementAndGet();
            }
        });
        GlobalScreen.postNativeEvent(new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_PRESSED, 0, 0,
                NativeKeyEvent.VC_CONTROL, NativeKeyEvent.CHAR_UNDEFINED, NativeKeyEvent.KEY_LOCATION_RIGHT));
        GlobalScreen.postNativeEvent(new NativeMouseWheelEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL,
                0, -12, 4, 0, NativeMouseWheelEvent.WHEEL_UNIT_SCROLL, 1, 0,
                NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION, 0.25));
        GlobalScreen.postNativeEvent(new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 10, 20, 0));
        check(delivered.get() == 3);
        check(GlobalScreen.getNativeMonitors().length == 0);
        try {
            GlobalScreen.postNativeEvent(new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_TYPED,
                    0, 0, NativeKeyEvent.VC_UNDEFINED, 'x'));
            throw new AssertionError("Expected post failure");
        } catch (Throwable error) {
            check(error instanceof NativeHookException);
        }
        try {
            GlobalScreen.postNativeEvent(null);
            throw new AssertionError("Expected null rejection");
        } catch (NullPointerException expected) { }
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeInputEvent(GlobalScreen.class, NativeKeyEvent.NATIVE_KEY_PRESSED, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL, 0, 0, 0, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(123, 0, 0, 0, 0)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 32768, 0, 0)));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeMouseEvent(NativeMouseEvent.NATIVE_MOUSE_MOVED, 0, 0, 0, 0) {
                    @Override public int getX() { throw new GetterFailure(); }
                }));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeInputEvent(GlobalScreen.class, 0, 0) {
                    @Override public int getID() { throw new GetterFailure(); }
                }));
        expect(GetterFailure.class, () -> GlobalScreen.postNativeEvent(
                new NativeKeyEvent(NativeKeyEvent.NATIVE_KEY_TYPED, 0, 0, 0, 'x') {
                    @Override public char getKeyChar() { throw new GetterFailure(); }
                }));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(Double.NaN)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(Double.POSITIVE_INFINITY)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(32769)));
        expect(IllegalArgumentException.class, () -> GlobalScreen.postNativeEvent(wheel(1e-20)));
        NativeMouseWheelEvent wheel = wheel(0.25);
        MouseWheelEvent swingWheel = new SwingMouseWheelAdapter() {
            MouseWheelEvent convert(NativeMouseWheelEvent event) { return getJavaMouseWheelEvent(event); }
        }.convert(wheel);
        check(swingWheel.getPreciseWheelRotation() == 0.25);
        check(swingWheel.getID() == MouseWheelEvent.MOUSE_WHEEL);

        AtomicReference<String> logged = new AtomicReference<>();
        Logger logger = Logger.getLogger("com.github.kwhat.jnativehook");
        logger.setUseParentHandlers(false);
        logger.addHandler(new Handler() {
            public void publish(LogRecord record) { logged.set(record.getMessage()); }
            public void flush() { }
            public void close() { }
        });
        emitFromNativeThread();
        check(delivered.get() == 4);
        check("UTF-8: \uD83D\uDE00".equals(logged.get()));
        StringBuilder typed = new StringBuilder();
        GlobalScreen.addNativeKeyListener(new NativeKeyListener() {
            public void nativeKeyTyped(NativeKeyEvent event) { typed.append(event.getKeyChar()); }
        });
        emitTypedScalar();
        check("\uD83D\uDE00".equals(typed.toString()));

        NativeMouseMotionListener throwing = new NativeMouseMotionListener() {
            public void nativeMouseMoved(NativeMouseEvent event) { throw new GetterFailure(); }
        };
        GlobalScreen.addNativeMouseMotionListener(throwing);
        // The callback error is reported, and the next event must still be delivered.
        emitFromNativeThread();
        GlobalScreen.removeNativeMouseMotionListener(throwing);
        emitFromNativeThread();
        check(delivered.get() == 5);
        String architecture = System.getProperty("os.arch");
        try {
            System.setProperty("os.arch", "arm64");
            check(NativeSystem.getArchitecture() == NativeSystem.Arch.ARM64);
            System.setProperty("os.arch", "aarch64");
            check(NativeSystem.getArchitecture() == NativeSystem.Arch.ARM64);
            System.setProperty("os.arch", "armv7l");
            check(NativeSystem.getArchitecture() == NativeSystem.Arch.ARM);
        } finally {
            System.setProperty("os.arch", architecture);
        }
        NativeMonitorInfo monitor = new NativeMonitorInfo((short) 0, 0, 0, (short) 40000, (short) 65535);
        check(monitor.getWidthPixels() == 40000 && monitor.getHeightPixels() == 65535);
        System.out.println("JNI API smoke test passed");
    }
    private static NativeMouseWheelEvent wheel(double rotation) {
        return new NativeMouseWheelEvent(NativeMouseEvent.NATIVE_MOUSE_WHEEL,
                0, -12, 4, 0, NativeMouseWheelEvent.WHEEL_UNIT_SCROLL, 1, 0,
                NativeMouseWheelEvent.WHEEL_HORIZONTAL_DIRECTION, rotation);
    }
    private static class GetterFailure extends RuntimeException { }
    private static void expect(Class<? extends Throwable> type, Runnable action) {
        try {
            action.run();
        } catch (Throwable error) {
            if (type.isInstance(error)) return;
            throw new AssertionError("Expected " + type.getName(), error);
        }
        throw new AssertionError("Expected " + type.getName());
    }
    private static void check(boolean condition) {
        if (!condition) throw new AssertionError();
    }
}
