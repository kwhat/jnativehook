/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.example;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FocusTraversalPolicy;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseListener;

// TODO: Auto-generated Javadoc
/**
 * The Class NativeHookDemo.
 */
public class NativeHookDemo extends JFrame implements KeyListener, NativeKeyListener, MouseListener, NativeMouseListener, ActionListener, FocusListener, WindowListener {
	
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -5549783775591314629L;
	
	/** The txt typing area. */
	private JTextField txtTypingArea;
	
	/** The txt event info. */
	private JTextArea txtEventInfo;
	
	/**
	 * Instantiates a new native hook demo.
	 */
	public NativeHookDemo() {
		setTitle("JNativeHook Demo");
		setLayout(new GridBagLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(300, 400);
		setFocusTraversalPolicy(new NoFocusTraversalPolicy());
		addWindowListener(this);
		
		txtTypingArea = new JTextField("Click to GrabKey");
		txtTypingArea.setEditable(false);
		txtTypingArea.setBackground(new Color(0xFF, 0xFF, 0xFF));
		txtTypingArea.setForeground(new Color(0x00, 0x00, 0x00));
		txtTypingArea.addFocusListener(this);
		
		
		txtEventInfo = new JTextArea();
		txtEventInfo.setEditable(false);
		txtEventInfo.setBackground(new Color(0xFF, 0xFF, 0xFF));
		txtEventInfo.setForeground(new Color(0x00, 0x00, 0x00));
		JScrollPane scrollPane = new JScrollPane(txtEventInfo);
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
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.
		txtEventInfo.setText("");
		txtTypingArea.setText("");
		
		//Return the focus to the window.
		this.requestFocusInWindow();	
	}
    
	/* (non-Javadoc)
	 * @see java.awt.event.FocusListener#focusGained(java.awt.event.FocusEvent)
	 */
	public void focusGained(FocusEvent e) {
		if (e.getSource() == txtTypingArea) {
			((JTextField) e.getSource()).setBackground(new Color(0xFF, 0xF9, 0xB6));
			((JTextField) e.getSource()).setText("Press Key to Bind");
			((JTextField) e.getSource()).addKeyListener(this);
			((JTextField) e.getSource()).addMouseListener(this);
		}
	}

	/* (non-Javadoc)
	 * @see java.awt.event.FocusListener#focusLost(java.awt.event.FocusEvent)
	 */
	public void focusLost(FocusEvent e) {
		if (e.getSource()  == txtTypingArea) {
			((JTextField) e.getSource()).removeKeyListener(this);
			((JTextField) e.getSource()).removeMouseListener(this);
			((JTextField) e.getSource()).setBackground(new Color(0xFF, 0xFF, 0xFF));
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
	 */
	public void keyTyped(KeyEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyPressed(java.awt.event.KeyEvent)
	 */
	public void keyPressed(KeyEvent e) {
		if (txtTypingArea.hasFocus()) {
			txtTypingArea.setText(getInputText(e));
			this.requestFocusInWindow();
		}
		else {
			displayEventInfo(e);
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyReleased(java.awt.event.KeyEvent)
	 */
	public void keyReleased(KeyEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	 */
	public void mouseClicked(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	 */
	public void mousePressed(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
		else {
			txtTypingArea.setText(getInputText(e));
			this.requestFocusInWindow();
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	 */
	public void mouseReleased(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
	 */
	public void mouseEntered(MouseEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
	 */
	public void mouseExited(MouseEvent e) { /* Do Nothing */ }
	
	/**
	 * Gets the input text.
	 *
	 * @param e the e
	 * @return the input text
	 */
	private String getInputText(InputEvent e) {
		String sReturn;
		switch (e.getID()) {
			case KeyEvent.KEY_PRESSED:
			case KeyEvent.KEY_RELEASED:
			case KeyEvent.KEY_TYPED:
				sReturn = "Keyboard: ";
				
				int code = ((KeyEvent) e).getKeyCode();
				if (code != KeyEvent.VK_SHIFT && 
					code != KeyEvent.VK_CONTROL &&
					code != KeyEvent.VK_ALT &&
					code != KeyEvent.VK_META &&
					e.getModifiers() != 0) {
					sReturn += KeyEvent.getKeyModifiersText(e.getModifiers()) + " + ";
				}
				
				int location = ((KeyEvent) e).getKeyLocation();
				if (location == KeyEvent.KEY_LOCATION_LEFT) {
					sReturn += " Left ";
				}
				else if (location == KeyEvent.KEY_LOCATION_RIGHT) {
					sReturn += " Right ";
				}
				
				sReturn += KeyEvent.getKeyText(code);
				
				try {
					GlobalScreen.getInstance().grabKey(e.getModifiers(), code, location);
				}
				catch (NativeKeyException ex) {
					ex.printStackTrace();
				}
			break;
			
			case MouseEvent.MOUSE_PRESSED:
			case MouseEvent.MOUSE_RELEASED:
			case MouseEvent.MOUSE_CLICKED:
				int button = ((MouseEvent) e).getButton();
				sReturn = "Mouse: Button" + button;
				
				try {
					GlobalScreen.getInstance().grabButton(button);
				}
				catch (NativeKeyException ex) {
					ex.printStackTrace();
				}
			break;
			
			default:
				sReturn = "" + e.getID();
		}
		
		return sReturn;
	}
	
	/**
	 * Display event info.
	 *
	 * @param e the e
	 */
	private void displayEventInfo(KeyEvent e) {
		String sEventText = txtEventInfo.getText() + "\n\n";;
		
		switch (e.getID()) {
			case KeyEvent.KEY_PRESSED:
				sEventText += "KEY_PRESSED:\n";
			break;
			
			case KeyEvent.KEY_RELEASED:
				sEventText += "KEY_RELEASED:\n";
			break;
			
			case KeyEvent.KEY_TYPED:
				sEventText += "KEY_PRESSED:\n";
			break;
			
			default:
				sEventText += "KEY_UNKNOWN:\n";
		}
		
		sEventText += "\tKeyCode:\t" + e.getKeyCode() + "\n";
		sEventText += "\tKeyText:\t" + KeyEvent.getKeyText(e.getKeyCode()) + "\n";
		sEventText += "\tLocation:\t" + e.getKeyLocation() + "\n";
		sEventText += "\tModifiers:\t" + KeyEvent.getKeyModifiersText(e.getModifiers()) + "\n";
		sEventText += "\n";
		
		txtEventInfo.setText(sEventText);
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
	}
	
	/**
	 * Display event info.
	 *
	 * @param e the e
	 */
	private void displayEventInfo(MouseEvent e) {
		String sEventText = txtEventInfo.getText() + "\n\n";
		
		switch (e.getID()) {
			case MouseEvent.MOUSE_PRESSED:
				sEventText += "MOUSE_PRESSED:\n";
			break;
			
			case MouseEvent.MOUSE_RELEASED:
				sEventText += "MOUSE_RELEASED:\n";
			break;
			
			case MouseEvent.MOUSE_CLICKED:
				sEventText += "MOUSE_CLICKED:\n";
			break;
			
			default:
				sEventText += "MOUSE_UNKNOWN:\n";
		}
		
		sEventText += "\tButton:\t" + e.getButton() + "\n";
		sEventText += "\tModifiers:\t" + KeyEvent.getKeyModifiersText(e.getModifiers()) + "\n";
		sEventText += "\n";
		
		txtEventInfo.setText(sEventText);
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
    }
    
	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args) {
		new NativeHookDemo();
	}
	
	/**
	 * The Class NoFocusTraversalPolicy.
	 */
	private class NoFocusTraversalPolicy extends FocusTraversalPolicy {
		
		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getComponentAfter(java.awt.Container, java.awt.Component)
		 */
		public Component getComponentAfter(Container container, Component component) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getComponentBefore(java.awt.Container, java.awt.Component)
		 */
		public Component getComponentBefore(Container container, Component component) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getDefaultComponent(java.awt.Container)
		 */
		public Component getDefaultComponent(Container container) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getFirstComponent(java.awt.Container)
		 */
		public Component getFirstComponent(Container container) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getLastComponent(java.awt.Container)
		 */
		public Component getLastComponent(Container container) {
			return null;
		}
	}

	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowActivated(java.awt.event.WindowEvent)
	 */
	public void windowActivated(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowClosing(java.awt.event.WindowEvent)
	 */
	public void windowClosing(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowDeactivated(java.awt.event.WindowEvent)
	 */
	public void windowDeactivated(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowDeiconified(java.awt.event.WindowEvent)
	 */
	public void windowDeiconified(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowIconified(java.awt.event.WindowEvent)
	 */
	public void windowIconified(WindowEvent e) { /* Do Nothing */ }

	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowOpened(java.awt.event.WindowEvent)
	 */
	public void windowOpened(WindowEvent e) {
		GlobalScreen.getInstance();
		GlobalScreen.getInstance().addNativeKeyListener(this);		
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowClosed(java.awt.event.WindowEvent)
	 */
	public void windowClosed(WindowEvent e) {
		System.runFinalization();
		System.exit(0);
	}
}
