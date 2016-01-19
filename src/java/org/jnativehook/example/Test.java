package org.jnativehook.example;

import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Test implements NativeMouseListener {

	public static void main(String[] args) throws InterruptedException {
		Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
		logger.setLevel(Level.WARNING);
		try {
			GlobalScreen.registerNativeHook();
		} catch (NativeHookException ex) {
			System.err.println(ex.getMessage());

			System.exit(1);
		}
		GlobalScreen.addNativeMouseListener(new Test());

		Thread.sleep(1000);
		NativeMouseEvent me = new  NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_CLICKED,
				System.currentTimeMillis(),
				0,
				75, 10,
				1, NativeMouseEvent.BUTTON1);
		GlobalScreen.postNativeEvent(me);

		//Thread.sleep(500);
		/*
		me = new  NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_RELEASED,
				System.currentTimeMillis(),
				0,
				50, 10,
				1, NativeMouseEvent.BUTTON1);

		GlobalScreen.postNativeEvent(me);
		*/
	}

	public void nativeMouseClicked(NativeMouseEvent arg0) {

	}

	public void nativeMousePressed(NativeMouseEvent e) {
		System.out.println(e.getX());
		System.out.println(e.getY());
	}

	public void nativeMouseReleased(NativeMouseEvent arg0) {
	}
}
