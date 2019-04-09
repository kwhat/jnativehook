module org.jnativehook {
	exports org.jnativehook;
	exports org.jnativehook.dispatcher;
	exports org.jnativehook.keyboard;
	exports org.jnativehook.mouse;

	requires transitive java.desktop;
	requires transitive java.logging;
}