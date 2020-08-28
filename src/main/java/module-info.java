module com.github.kwhat.jnativehook {
	exports com.github.kwhat.jnativehook;
	exports com.github.kwhat.jnativehook.dispatcher;
	exports com.github.kwhat.jnativehook.keyboard;
	exports com.github.kwhat.jnativehook.mouse;

	requires transitive java.desktop;
	requires transitive java.logging;
}
