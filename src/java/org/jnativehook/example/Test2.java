package org.jnativehook.example;

import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Test2 implements NativeKeyListener {

	public static void main(String[] args) throws InterruptedException {
		Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
		logger.setLevel(Level.WARNING);

		try {
			GlobalScreen.registerNativeHook();
		} catch (NativeHookException ex) {
			System.err.println(ex.getMessage());

			System.exit(1);
		}

		NativeKeyEvent ev = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VC_MEDIA_PLAY,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		GlobalScreen.addNativeKeyListener(new Test2());

		GlobalScreen.postNativeEvent(ev);
	}

	public void nativeKeyPressed(NativeKeyEvent e) {
		System.out.println("Pressed: " + e.getKeyCode() + ":" + e.getRawCode());
	}

	public void nativeKeyReleased(NativeKeyEvent e) {

	}

	public void nativeKeyTyped(NativeKeyEvent e) {

	}
}
