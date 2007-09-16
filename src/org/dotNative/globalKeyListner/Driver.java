package org.dotNative.globalKeyListner;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import org.dotNative.globalKeyListner.KeyboardEvent;
import org.dotNative.globalKeyListner.KeyboardEventListener;
import org.dotNative.globalKeyListner.KeyboardHook;

//Should go GPL

public class Driver implements KeyboardEventListener {
	public static void main( String args[] ) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		KeyboardHook kh = new KeyboardHook();
		kh.addEventListener( new Driver() );
		BufferedReader br = new BufferedReader( new InputStreamReader( System.in ) );
		try{
			br.readLine();
		} catch( IOException ex ) {}
	}
 
	public void globalKeyPressed( KeyboardEvent event )	{
		System.out.println( "Key Pressed: " + event.getVirtualKeyCode() );
	}
 
	public void globalKeyReleased( KeyboardEvent event ) {
		System.out.println( "Key Released: " + event.getVirtualKeyCode() );
	}
}
