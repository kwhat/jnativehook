package org.jnativehook.keyboard;

public class NativeKeyException extends Exception {
	private static final long serialVersionUID = -6239432803200521634L;

	public NativeKeyException() {
		super();
	}
	
	public NativeKeyException(String sMessage) {
		super(sMessage);
	}
	
	public NativeKeyException(String sMessage, Throwable objThrowable) {
		super(sMessage, objThrowable);
	}
	
	public NativeKeyException(Throwable objThrowable) {
		super(objThrowable);
	}
}
