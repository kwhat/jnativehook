package org.jnativehook.example;

import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseMotionListener;

public class NativeHookCLITest implements NativeMouseMotionListener {
	
	public NativeHookCLITest() {
		GlobalScreen.getInstance().addNativeMouseMotionListener(this);
	}
	
	public void mouseMoved(NativeMouseEvent e) {
		System.out.println(e.paramString());
	}

	public static void main(String[] args) throws NativeHookException, InterruptedException {
		GlobalScreen.getInstance().registerNativeHook();
		
		new NativeHookCLITest();
		Thread.sleep(10 * 1000);
		
		
		GlobalScreen.getInstance().unregisterNativeHook();
		Thread.sleep(5 * 1000);
	}
}
