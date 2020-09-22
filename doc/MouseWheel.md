### Global Mouse Wheel Listener
```java
import GlobalScreen;
import NativeHookException;
import NativeMouseWheelEvent;
import NativeMouseWheelListener;

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
