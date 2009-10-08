package org.jnativehook.example;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class KeyEventDemo extends JFrame implements KeyListener, MouseListener, ActionListener, FocusListener {
	JTextArea displayArea;
	JTextField typingArea;
	
	public KeyEventDemo() {
		setTitle("JNativeHook Demo");
		setLayout(new GridBagLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(300, 400);
		
		typingArea = new JTextField("Click to GrabKey");
		typingArea.setEditable(false);
		typingArea.addFocusListener(this);
		typingArea.setFocusTraversalKeysEnabled(false);
		
		displayArea = new JTextArea();
		displayArea.setEditable(false);
		JScrollPane scrollPane = new JScrollPane(displayArea);
		scrollPane.setPreferredSize(new Dimension(375, 125));
		
		GridBagConstraints c = new GridBagConstraints();
		c.insets = new Insets(5, 5, 5, 5);
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.NORTHWEST;
		c.weightx = 1.0;
		c.weighty = 0.0;
		c.gridx = 0;
		c.gridy = 0;
		add(typingArea, c);
		
		c.gridy = 1;
		c.fill = GridBagConstraints.BOTH;
		c.weighty = 1.0;
		add(scrollPane, c);
		
		setVisible(true);
	}
	
	/** Handle the key typed event from the text field. */
	public void keyTyped(KeyEvent e) {
		displayInfo(e, "KEY TYPED: ");
	}
	
	/** Handle the key pressed event from the text field. */
	public void keyPressed(KeyEvent e) {
		displayInfo(e, "KEY PRESSED: ");
	}
	
	/** Handle the key released event from the text field. */
	public void keyReleased(KeyEvent e) {
		displayInfo(e, "KEY RELEASED: ");
	}
    
	/** Handle the button click. */
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.

		displayArea.setText("");
		typingArea.setText("");
		
		//	Return the focus to the typing area.
		typingArea.requestFocusInWindow();
	
	}
    
	public void focusGained(FocusEvent e) {
		if (e.getSource().equals(typingArea)) {
			((JTextField) e.getSource()).setText("");
			((JTextField) e.getSource()).addActionListener(this);
			((JTextField) e.getSource()).addMouseListener(this);
		}
	}

	public void focusLost(FocusEvent e) {
		if (e.getSource().equals(typingArea)) {
			((JTextField) e.getSource()).removeActionListener(this);
			((JTextField) e.getSource()).removeMouseListener(this);
		}
	}
	

	@Override
	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}
	
	public void mouseEntered(MouseEvent e) { /* Do Nothing */ }
	public void mouseExited(MouseEvent e) { /* Do Nothing */ }
	public void mousePressed(MouseEvent e) { /* Do Nothing */ }
	public void mouseReleased(MouseEvent e) { /* Do Nothing */ }
	
	private void displayInfo(KeyEvent e, String keyStatus){
		//You should only rely on the key char if the event
		//is a key typed event.
		int id = e.getID();
		String keyString;
		if (id == KeyEvent.KEY_TYPED) {
		    char c = e.getKeyChar();
		    keyString = "key character = '" + c + "'";
		}
		else {
			int keyCode = e.getKeyCode();
			keyString = "key code = " + keyCode + " ("
				+ KeyEvent.getKeyText(keyCode)
				+ ")";
		}
		
		int modifiersEx = e.getModifiersEx();
		String modString = "extended modifiers = " + modifiersEx;
		String tmpString = KeyEvent.getModifiersExText(modifiersEx);
		if (tmpString.length() > 0) {
		    modString += " (" + tmpString + ")";
		} else {
		    modString += " (no extended modifiers)";
		}
		
		String actionString = "action key? ";
		if (e.isActionKey()) {
		    actionString += "YES";
		}
		else {
		    actionString += "NO";
		}
		
		String locationString = "key location: ";
		int location = e.getKeyLocation();
		if (location == KeyEvent.KEY_LOCATION_STANDARD) {
		    locationString += "standard";
		}
		else if (location == KeyEvent.KEY_LOCATION_LEFT) {
		    locationString += "left";
		}
		else if (location == KeyEvent.KEY_LOCATION_RIGHT) {
		    locationString += "right";
		}
		else if (location == KeyEvent.KEY_LOCATION_NUMPAD) {
		    locationString += "numpad";
		}
		else { // (location == KeyEvent.KEY_LOCATION_UNKNOWN)
			locationString += "unknown";
		}
		
		String newline = System.getProperty("line.separator");
		displayArea.append(keyStatus + newline
				+ "    " + keyString + newline
				+ "    " + modString + newline
				+ "    " + actionString + newline
				+ "    " + locationString + newline);
		displayArea.setCaretPosition(displayArea.getDocument().getLength());
    }
    
	public static void main(String[] args) {
		new KeyEventDemo();
	}
}
