package org.dotnative.globalhook.keyboard;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.dotnative.globalhook.keyboard.GlobalKeyEvent;
import org.dotnative.globalhook.keyboard.GlobalKeyHook;
import org.dotnative.globalhook.keyboard.GlobalKeyListener;

//Should go GPL

public class Driver implements GlobalKeyListener {
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		//GlobalKeyHook objKeyHook = new GlobalKeyHook("/home/kwhat/workspace/dotNative/dist/");
		GlobalKeyHook objKeyHook = new GlobalKeyHook();
		objKeyHook.addEventListener(new Driver());
		
		/*
		BufferedReader br = new BufferedReader( new InputStreamReader( System.in ) );
		try{
			br.readLine();
		} catch( IOException ex ) {}
		*/
	}
	
	public void globalKeyPressed( GlobalKeyEvent event )	{
		System.out.println("Key Pressed: " + event.getVirtualKeyCode());
	}
	
	public void globalKeyReleased( GlobalKeyEvent event ) {
		System.out.println("Key Released: " + event.getVirtualKeyCode());
	}
}
