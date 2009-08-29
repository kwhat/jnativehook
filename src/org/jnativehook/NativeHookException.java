package org.jnativehook;

public class NativeHookException extends RuntimeException {
	private static final long serialVersionUID = 6199753732102764333L;

	public NativeHookException() {
		super();
	}
	
	public NativeHookException(String sMessage) {
		super(sMessage);
	}
	
	public NativeHookException(String sMessage, Throwable objThrowable) {
		super(sMessage, objThrowable);
	}
	
	public NativeHookException(Throwable objThrowable) {
		super(objThrowable);
	}
}
