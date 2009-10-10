package org.jnativehook.example;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FocusTraversalPolicy;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class KeyEventDemo extends JFrame implements KeyListener, MouseListener, ActionListener, FocusListener {
	private boolean bGrabbing;
	private JButton btnSetKey;
	private JTextField txtTypingArea;
	private JTextArea txtDisplayArea;
	
	public KeyEventDemo() {
		setTitle("JNativeHook Demo");
		setLayout(new GridBagLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(300, 400);
		setFocusTraversalPolicy(new NoFocusTraversalPolicy());
		
		bGrabbing = false;
		btnSetKey = new JButton("Set Bind");
		
		txtTypingArea = new JTextField("Click to GrabKey");
		txtTypingArea.setEditable(false);
		txtTypingArea.setBackground(Color.WHITE);
		txtTypingArea.setForeground(Color.BLACK);
		txtTypingArea.addFocusListener(this);
		
		//typingArea.setFocusTraversalKeysEnabled(false);
		//FocusTraversalPolicy.getDefaultComponent(this).requestFocus();
		
		
		txtDisplayArea = new JTextArea();
		txtDisplayArea.setEditable(false);
		JScrollPane scrollPane = new JScrollPane(txtDisplayArea);
		scrollPane.setPreferredSize(new Dimension(375, 125));
		
		GridBagConstraints c = new GridBagConstraints();
		c.insets = new Insets(5, 5, 5, 5);
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.NORTHWEST;
		c.weightx = 1.0;
		c.weighty = 0.0;
		c.gridx = 0;
		c.gridy = 0;
		add(txtTypingArea, c);
		
		c.gridy = 1;
		c.fill = GridBagConstraints.BOTH;
		c.weighty = 1.0;
		add(scrollPane, c);
		
		setVisible(true);
	}
	
	/** Handle the button click. */
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.

		txtDisplayArea.setText("");
		txtTypingArea.setText("");
		
		//	Return the focus to the typing area.
		txtTypingArea.requestFocusInWindow();
	
	}
    
	public void focusGained(FocusEvent e) {
		System.out.println("Got Focus");
		
		if (e.getSource() == txtTypingArea) {
			((JTextField) e.getSource()).setText("Press Key to Bind");
			((JTextField) e.getSource()).addKeyListener(this);
			((JTextField) e.getSource()).addMouseListener(this);
		}
	}

	public void focusLost(FocusEvent e) {
		System.out.println("Lost Focus");
		
		if (e.getSource()  == txtTypingArea) {
			((JTextField) e.getSource()).removeKeyListener(this);
			((JTextField) e.getSource()).removeMouseListener(this);
		}
	}
	
	public void keyTyped(KeyEvent e) {
		if (!bGrabbing) {
			//displayInfo(e, "KEY TYPED: ");
			//this.requestFocusInWindow();
		}
	}
	
	public void keyPressed(KeyEvent e) {
		if (!bGrabbing) {
			displayInfo(e, "KEY PRESSED: ");
			this.requestFocusInWindow();
		}
		else {
			txtTypingArea.setText(KeyEvent.getKeyModifiersText(e.getModifiers()) + " + " + KeyEvent.getKeyText(e.getKeyCode()));
			this.requestFocusInWindow();
		}
	}
	
	public void keyReleased(KeyEvent e) {
		if (!bGrabbing) {
			displayInfo(e, "KEY RELEASED: ");
		}
	}
	
	public void mouseClicked(MouseEvent e) {
		if (!bGrabbing)
		{
			
			int modifiers =e.getModifiers(),
				button =e.getButton();
			Point pt =e.getLocationOnScreen();
			
			String newline = System.getProperty("line.separator");
			txtDisplayArea.append("Mouse Clicked: " + newline +
								"\tModifiers: " + modifiers + newline
								+ "\tButton: " + button + newline
								+ "\tPoint: (" + pt.x + "," + pt.y + ")" + newline);
			
			txtTypingArea.setText("Mouse Button: " + button);
			/*
			String newline = System.getProperty("line.separator");
			txtDisplayArea.append(keyStatus + newline
					+ "    " + keyString + newline
					+ "    " + modString + newline
					+ "    " + actionString + newline
					+ "    " + locationString + newline);
			txtDisplayArea.setCaretPosition(txtDisplayArea.getDocument().getLength());
			*/
		}
	}
	public void mousePressed(MouseEvent e) {
		if (!bGrabbing) {
			//displayInfo(e, "KEY RELEASED: ");
		}
		else {
			txtTypingArea.setText(MouseEvent.getMouseModifiersText(e.getModifiers()) + " + " + "MOUSE_BUTTON_" + e.getButton());
			this.requestFocusInWindow();
		}
	}
	public void mouseReleased(MouseEvent e) { /* Do Nothing */ }
	
	public void mouseEntered(MouseEvent e) { /* Do Nothing */ }
	public void mouseExited(MouseEvent e) { /* Do Nothing */ }
	
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
			keyString = "key code = " + keyCode + 
				" (" + KeyEvent.getKeyText(keyCode) + ")";
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
		txtDisplayArea.append(keyStatus + newline
				+ "    " + keyString + newline
				+ "    " + modString + newline
				+ "    " + actionString + newline
				+ "    " + locationString + newline);
		txtDisplayArea.setCaretPosition(txtDisplayArea.getDocument().getLength());
		
		//update top text area
		String text ="";
		
		switch (e.getKeyLocation())
		{
			case KeyEvent.KEY_LOCATION_LEFT:
				text += "(Left)";
				break;
				
			case KeyEvent.KEY_LOCATION_RIGHT:
				text += "(Right)";
				break;
				
			case KeyEvent.KEY_LOCATION_NUMPAD:
				text += "(Numpad)";
				break;
				
			case KeyEvent.KEY_LOCATION_STANDARD:
				text += "(Standard)";
				break;
				
			default:
				text += "(Unknown)";
		}
		
		
		if (tmpString.length() >0)
		{
			int keyCode = e.getKeyCode();
			txtTypingArea.setText("(" + tmpString + ") " + KeyEvent.getKeyText(keyCode));
			
			text += " (" + tmpString + ") " + KeyEvent.getKeyText(keyCode);
		}
		else
		{
			int keyCode = e.getKeyCode();
			
			text += KeyEvent.getKeyText(keyCode);
			
			//txtTypingArea.setText(KeyEvent.getKeyText(keyCode));
			
			//txtTypingArea.setText(KeyEvent.k)
		}
		
		txtTypingArea.setText(text);
		
    }
    
	public static void main(String[] args) {
		new KeyEventDemo();
	}
	
	private class NoFocusTraversalPolicy extends FocusTraversalPolicy {
		public Component getComponentAfter(Container container, Component component) {
			return null;
		}

		public Component getComponentBefore(Container container, Component component) {
			return null;
		}

		public Component getDefaultComponent(Container container) {
			return null;
		}

		public Component getFirstComponent(Container container) {
			return null;
		}

		public Component getLastComponent(Container container) {
			return null;
		}
	}
}
