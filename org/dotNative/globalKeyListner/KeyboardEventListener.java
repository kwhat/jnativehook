package org.dotNative.globalKeyListner;


//KeyboardEventListener.java

import java.util.EventListener;


public interface KeyboardEventListener extends EventListener {
	public void globalKeyPressed( KeyboardEvent event );
	public void globalKeyReleased( KeyboardEvent event );
}
