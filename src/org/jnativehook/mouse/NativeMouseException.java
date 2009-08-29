package org.jnativehook.mouse;

public class NativeMouseException extends Exception {
	private static final long serialVersionUID = -6239432803200521634L;

	public NativeMouseException() {
		super();
	}
	
	public NativeMouseException(String sMessage) {
		super(sMessage);
	}
	
	public NativeMouseException(String sMessage, Throwable objThrowable) {
		super(sMessage, objThrowable);
	}
	
	public NativeMouseException(Throwable objThrowable) {
		super(objThrowable);
	}
}
