### Demo Application
  The JNativeHook library ships with a demo application containing most available features.  [src/java/org/jnativehook/example/NativeHookDemo.java](https://github.com/kwhat/jnativehook/blob/master/src/java/org/jnativehook/example/NativeHookDemo.java)


### Global Keyboard Listener
```java
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;

public class GlobalKeyListenerExample implements NativeKeyListener {
	public void nativeKeyPressed(NativeKeyEvent e) {
		System.out.println("Key Pressed: " + NativeKeyEvent.getKeyText(e.getKeyCode()));

		if (e.getKeyCode() == NativeKeyEvent.VK_ESCAPE) {
			GlobalScreen.unregisterNativeHook();
		}
	}

	public void nativeKeyReleased(NativeKeyEvent e) {
		System.out.println("Key Released: " + NativeKeyEvent.getKeyText(e.getKeyCode()));
	}

	public void nativeKeyTyped(NativeKeyEvent e) {
		System.out.println("Key Typed: " + e.getKeyText(e.getKeyCode()));
	}

	public static void main(String[] args) {
		try {
			GlobalScreen.registerNativeHook();
		}
		catch (NativeHookException ex) {
			System.err.println("There was a problem registering the native hook.");
			System.err.println(ex.getMessage());

			System.exit(1);
		}

		GlobalScreen.addNativeKeyListener(new GlobalKeyListenerExample());
	}
}
```

### Global Mouse Listener
```java
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseInputListener;

public class GlobalMouseListenerExample implements NativeMouseInputListener {
	public void nativeMouseClicked(NativeMouseEvent e) {
		System.out.println("Mouse Clicked: " + e.getClickCount());
	}

	public void nativeMousePressed(NativeMouseEvent e) {
		System.out.println("Mouse Pressed: " + e.getButton());
	}

	public void nativeMouseReleased(NativeMouseEvent e) {
		System.out.println("Mouse Released: " + e.getButton());
	}

	public void nativeMouseMoved(NativeMouseEvent e) {
		System.out.println("Mouse Moved: " + e.getX() + ", " + e.getY());
	}

	public void nativeMouseDragged(NativeMouseEvent e) {
		System.out.println("Mouse Dragged: " + e.getX() + ", " + e.getY());
	}

	public static void main(String[] args) {
		try {
			GlobalScreen.registerNativeHook();
		}
		catch (NativeHookException ex) {
			System.err.println("There was a problem registering the native hook.");
			System.err.println(ex.getMessage());

			System.exit(1);
		}

		// Construct the example object.
		GlobalMouseListenerExample example = new GlobalMouseListenerExample();

		// Add the appropriate listeners.
		GlobalScreen.addNativeMouseListener(example);
		GlobalScreen.addNativeMouseMotionListener(example);
	}
}
```

### Global Mouse Wheel Listener
```java
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

public class GlobalMouseWheelListenerExample implements NativeMouseWheelListener {
	public void nativeMouseWheelMoved(NativeMouseWheelEvent e) {
		System.out.println("Mosue Wheel Moved: " + e.getWheelRotation());
	}

	public static void main(String[] args) {
		try {
			GlobalScreen.registerNativeHook();
		}
		catch (NativeHookException ex) {
			System.err.println("There was a problem registering the native hook.");
			System.err.println(ex.getMessage());
			ex.printStackTrace();

			System.exit(1);
		}

		GlobalScreen.addNativeMouseWheelListener(new GlobalMouseWheelListenerExample());
	}
}
```

### Working with Swing
JNativeHook does *NOT* operate on the event dispatch thread.  Because Swing components are not thread safe, you *MUST* wrap access to Swing components using the [SwingUtilities.invokeLater()](http://docs.oracle.com/javase/1.5.0/docs/api/javax/swing/SwingUtilities.html#invokeLater(java.lang.Runnable)) or [EventQueue.invokeLater()](http://docs.oracle.com/javase/1.5.0/docs/api/java/awt/EventQueue.html#invokeLater(java.lang.Runnable)) methods.  As of version 2.0, the `SwingDispatchService` class maybe used to automatically use Swing's event dispatch thread for all native event delivery.

```java
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.SwingDispatchService;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;

public class SwingExample extends JFrame implements NativeKeyListener, WindowListener {
	public SwingExample() {
		// Set the event dispatcher to a swing safe executor service.
		GlobalScreen.setEventDispatcher(new SwingDispatchService());

		setTitle("JNativeHook Swing Example");
		setSize(300, 150);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		addWindowListener(this);
		setVisible(true);
	}

	public void windowOpened(WindowEvent e) {
		// Initialze native hook.
		try {
			GlobalScreen.registerNativeHook();
		}
		catch (NativeHookException ex) {
			System.err.println("There was a problem registering the native hook.");
			System.err.println(ex.getMessage());
			ex.printStackTrace();

			System.exit(1);
		}

		GlobalScreen.addNativeKeyListener(this);
	}

	public void windowClosed(WindowEvent e) {
		//Clean up the native hook.
		GlobalScreen.unregisterNativeHook();
		System.runFinalization();
		System.exit(0);
	}

	public void windowClosing(WindowEvent e) { /* Unimplemented */ }
	public void windowIconified(WindowEvent e) { /* Unimplemented */ }
	public void windowDeiconified(WindowEvent e) { /* Unimplemented */ }
	public void windowActivated(WindowEvent e) { /* Unimplemented */ }
	public void windowDeactivated(WindowEvent e) { /* Unimplemented */ }

	public void nativeKeyReleased(NativeKeyEvent e) {
		if (e.getKeyCode() == NativeKeyEvent.VK_SPACE) {
			JOptionPane.showMessageDialog(null, "This will run on Swing's Event Dispatch Thread.");
		}
	}

	public void nativeKeyPressed(NativeKeyEvent e) { /* Unimplemented */ }
	public void nativeKeyTyped(NativeKeyEvent e) { /* Unimplemented */ }

	public static void main(String[] args) {
		 SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new SwingExample();
			}
		});
	}
}
```


### Logging and Console Output
As of version 2.0, JNativeHook includes an internal [Logger](http://docs.oracle.com/javase/1.5.0/docs/api/java/util/logging/Logger.html) to handle nearly all of the console output.  By default, JNativeHook will use the default Logger configured by the JVM.

**Set the log level to only display warnings and errors.**
```java
// Get the logger for "org.jnativehook" and set the level to warning.
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.WARNING);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```

**Disable all console output.**
```java
// Get the logger for "org.jnativehook" and set the level to off.
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.OFF);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```

**Set the log level to display everything.**
```java
// Get the "org.jnativehook" logger and set the level
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.ALL);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```

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