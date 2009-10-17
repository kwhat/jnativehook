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

public class NativeHookDemo extends JFrame implements KeyListener, NativeKeyListener, MouseListener, NativeMouseListener, ActionListener, FocusListener, WindowListener {
	private static final long serialVersionUID = -5549783775591314629L;
	private JTextField txtTypingArea;
	private JTextArea txtEventInfo;
	
	public NativeHookDemo() {
		setTitle("JNativeHook Demo");
		setLayout(new GridBagLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(300, 400);
		setFocusTraversalPolicy(new NoFocusTraversalPolicy());
		addWindowListener(this);
		
		txtTypingArea = new JTextField("Click to GrabKey");
		txtTypingArea.setEditable(false);
		txtTypingArea.setBackground(Color.GRAY);
		txtTypingArea.setForeground(Color.WHITE);
		txtTypingArea.addFocusListener(this);
		
		
		txtEventInfo = new JTextArea();
		txtEventInfo.setEditable(false);
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
	
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.
		txtEventInfo.setText("");
		txtTypingArea.setText("");
		
		//Return the focus to the window.
		this.requestFocusInWindow();	
	}
    
	public void focusGained(FocusEvent e) {
		if (e.getSource() == txtTypingArea) {
			((JTextField) e.getSource()).setText("Press Key to Bind");
			((JTextField) e.getSource()).addKeyListener(this);
			((JTextField) e.getSource()).addMouseListener(this);
		}
	}

	public void focusLost(FocusEvent e) {
		if (e.getSource()  == txtTypingArea) {
			((JTextField) e.getSource()).removeKeyListener(this);
			((JTextField) e.getSource()).removeMouseListener(this);
		}
	}
	
	public void keyTyped(KeyEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	public void keyPressed(KeyEvent e) {
		if (txtTypingArea.hasFocus()) {
			txtTypingArea.setText(getInputText(e));
			this.requestFocusInWindow();
		}
		else {
			displayEventInfo(e);
		}
	}
	
	public void keyReleased(KeyEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	public void mouseClicked(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	public void mousePressed(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
		else {
			txtTypingArea.setText(getInputText(e));
			this.requestFocusInWindow();
		}
	}
	
	public void mouseReleased(MouseEvent e) {
		if (!txtTypingArea.hasFocus()) {
			displayEventInfo(e);
		}
	}
	
	public void mouseEntered(MouseEvent e) { /* Do Nothing */ }
	public void mouseExited(MouseEvent e) { /* Do Nothing */ }
	
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
    
	public static void main(String[] args) {
		new NativeHookDemo();
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

	public void windowActivated(WindowEvent e) { /* Do Nothing */ }
	public void windowClosing(WindowEvent e) { /* Do Nothing */ }
	public void windowDeactivated(WindowEvent e) { /* Do Nothing */ }
	public void windowDeiconified(WindowEvent e) { /* Do Nothing */ }
	public void windowIconified(WindowEvent e) { /* Do Nothing */ }

	public void windowOpened(WindowEvent e) {
		GlobalScreen.getInstance();
		GlobalScreen.getInstance().addNativeKeyListener(this);		
	}
	
	public void windowClosed(WindowEvent e) {
		System.runFinalization();
		System.exit(0);
	}
}
