import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import jni.keyboard.KeyboardEvent;
import jni.keyboard.KeyboardEventListener;
import jni.keyboard.KeyboardHook;

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
 
	public void GlobalKeyPressed( KeyboardEvent event )	{
		System.out.println( "Key Pressed: " + event.getVirtualKeyCode() );
	}
 
	public void GlobalKeyReleased( KeyboardEvent event ) {
		System.out.println( "Key Released: " + event.getVirtualKeyCode() );
	}
}
