package org.dotnative.globalhook.keyboard;

//Imports
import java.util.EventListener;

//KeyboardEventListener.java

public interface GlobalKeyListener extends EventListener {
	public void globalKeyPressed(GlobalKeyEvent objEvent);
	public void globalKeyReleased(GlobalKeyEvent objEvent);
}
