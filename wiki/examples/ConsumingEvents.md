### Consuming Events (Unsupported)
Starting with version 2.0 it is now possible to prevent event propagation by consuming native events from Java.  This behavior is currently unsupported because it is impossible to implement on X11 platforms like Unix, Linux and Solaris.  This may change in the future as Wayland begins to replace X11 as the default windowing server on these platforms.

```java
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.NativeInputEvent;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ConsumeEvent implements NativeKeyListener {
	public ConsumeEvent() throws NativeHookException {
		// Create custom logger and level.
		Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
		logger.setLevel(Level.WARNING);

		GlobalScreen.setEventDispatcher(new VoidDispatchService());
		GlobalScreen.registerNativeHook();

		GlobalScreen.addNativeKeyListener(this);
	}

	private class VoidDispatchService extends AbstractExecutorService {
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

		public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
			return true;
		}

		public void execute(Runnable r) {
			r.run();
		}
	}

	public void nativeKeyPressed(NativeKeyEvent e) {
		if (e.getKeyCode() == NativeKeyEvent.VC_B) {
			System.out.print("Attempting to consume B event...\t");
			try {
				Field f = NativeInputEvent.class.getDeclaredField("reserved");
				f.setAccessible(true);
				f.setShort(e, (short) 0x01);

				System.out.print("[ OK ]\n");
			}
			catch (Exception ex) {
				System.out.print("[ !! ]\n");
				ex.printStackTrace();
			}
		}
	}

	public void nativeKeyReleased(NativeKeyEvent e) {
		if (e.getKeyCode() == NativeKeyEvent.VC_B) {
			System.out.print("Attempting to consume B event...\t");
			try {
				Field f = NativeInputEvent.class.getDeclaredField("reserved");
				f.setAccessible(true);
				f.setShort(e, (short) 0x01);

				System.out.print("[ OK ]\n");
			}
			catch (Exception ex) {
				System.out.print("[ !! ]\n");
				ex.printStackTrace();
			}
		}
	}

	public void nativeKeyTyped(NativeKeyEvent e) { /* Unimplemented */ }

	public static void main(String [] args) throws NativeHookException {
		new ConsumeEvent();
	}
}
```
