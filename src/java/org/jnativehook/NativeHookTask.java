package org.jnativehook;

import java.util.concurrent.Callable;

public class NativeHookTask implements Callable<Integer> {
	public Integer call() throws Exception {
		Integer status = enable();

		return status;
	}

	protected native int enable();
}
