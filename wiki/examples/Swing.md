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
