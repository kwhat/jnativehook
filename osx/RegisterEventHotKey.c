#include <Carbon/Carbon.h>

OSStatus handler(EventHandlerCallRef nextHandler, EventRef theEvent, void* userData) {
  printf("Global keyboard shortcut pressed!\n");
  return noErr;
}

int main() {
  EventTypeSpec eventType;
  eventType.eventClass = kEventClassKeyboard;
  eventType.eventKind  = kEventHotKeyPressed;
  
  InstallApplicationEventHandler(&handler, 1, &eventType, NULL, NULL);
  
  EventHotKeyID g_HotKeyID;
  g_HotKeyID.id = 1;

  EventHotKeyRef g_HotKeyRef;

  //http://boredzo.org/blog/wp-content/uploads/2007/05/IMTx-virtual-keycodes.pdf
  //Keycode 11 is the B key.
  RegisterEventHotKey(11, controlKey + cmdKey, g_HotKeyID, GetApplicationEventTarget(), 0, &g_HotKeyRef);

  printf("Press the global keyboard shortcut!\n");

  RunApplicationEventLoop();

  return 0;
}
