package org.jnativehook.example;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JFrame;
import javax.swing.JTextField;

public class KeyTest {
	public static void main(String args[]) {
		JFrame frame = new JFrame("Key Listener");
		Container contentPane = frame.getContentPane();

		KeyListener listener = new KeyListener() {
			public void keyPressed(KeyEvent e) {
				dumpInfo("Pressed", e);
			}

			public void keyReleased(KeyEvent e) {
				dumpInfo("Released", e);
			}

			public void keyTyped(KeyEvent e) {
				dumpInfo("Typed", e);
			}

			private void dumpInfo(String s, KeyEvent e) {
				System.out.println(s);
				int code = e.getKeyCode();
				System.out.println("\tCode: " + KeyEvent.getKeyText(code));
				System.out.println("\tChar: " + e.getKeyChar());
				int mods = e.getModifiersEx();
				System.out.println("\tMods: "
						+ KeyEvent.getModifiersExText(mods));
				System.out.println("\tLocation: "
						+ location(e.getKeyLocation()));
				System.out.println("\tAction? " + e.isActionKey());
			}

			private String location(int location) {
				switch (location) {
				case KeyEvent.KEY_LOCATION_LEFT:
					return "Left";
				case KeyEvent.KEY_LOCATION_RIGHT:
					return "Right";
				case KeyEvent.KEY_LOCATION_NUMPAD:
					return "NumPad";
				case KeyEvent.KEY_LOCATION_STANDARD:
					return "Standard";
				case KeyEvent.KEY_LOCATION_UNKNOWN:
				default:
					return "Unknown";
				}
			}
		};

		JTextField text = new JTextField();
		text.addKeyListener(listener);
		contentPane.add(text, BorderLayout.NORTH);
		frame.pack();
		frame.show();
	}
}
