### Logging and Console Output
As of version 2.0, JNativeHook includes an internal [Logger](http://docs.oracle.com/javase/1.5.0/docs/api/java/util/logging/Logger.html) 
to handle nearly all console output.  By default, JNativeHook will use the default Logger 
configured by the JVM.

**Set the log level to only display warnings and errors.**
```java
// Get the logger for "com.github.kwhat.jnativehook" and set the level to warning.
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.WARNING);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```

**Disable all console output.**
```java
// Get the logger for "com.github.kwhat.jnativehook" and set the level to off.
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.OFF);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```

**Set the log level to display everything.**
```java
// Get the "com.github.kwhat.jnativehook" logger and set the level
Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
logger.setLevel(Level.ALL);

// Don't forget to disable the parent handlers.
logger.setUseParentHandlers(false);
```
