/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.example;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.ItemSelectable;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.BorderFactory;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.WindowConstants;
import javax.swing.border.EtchedBorder;
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseInputListener;

/**
 * A demonstration of how to use the JNativeHook library.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 * 
 * @see GlobalScreen
 * @see NativeKeyListener
 */
public class NativeHookDemo extends JFrame implements NativeKeyListener, NativeMouseInputListener, ActionListener, WindowListener, ItemListener {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -5076634313730799059L;
	
	/** Checkbox's for event delivery options. */
	private JCheckBox chkKeyboard, chkButton, chkMotion;
	
	/** The text area to display event info. */
	private JTextArea txtEventInfo;
	
	/**
	 * Instantiates a new native hook demo.
	 */
	public NativeHookDemo() {
		try {
			//Initialze native hook.
			GlobalScreen.getInstance().registerNativeHook();
		}
		catch (NativeHookException ex) {
			ex.printStackTrace();
		}
		
		//Setup the main window.
		setTitle("JNativeHook Demo");
		setLayout(new BorderLayout());
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		setSize(600, 300);
		addWindowListener(this);
		
		//Create options panel
		JPanel grpOptions = new JPanel();
		grpOptions.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		grpOptions.setLayout(new FlowLayout(FlowLayout.LEADING));
		add(grpOptions, BorderLayout.NORTH);
		
		//Create keyboard checkbox
		chkKeyboard = new JCheckBox("Keyboard Events");
		chkKeyboard.setMnemonic(KeyEvent.VK_K);
		chkKeyboard.addItemListener(this);
		chkKeyboard.setSelected(true);
		grpOptions.add(chkKeyboard);
		
		//Create button checkbox
		chkButton = new JCheckBox("Button Events");
		chkButton.setMnemonic(KeyEvent.VK_B);
		chkButton.addItemListener(this);
		chkButton.setSelected(true);
		grpOptions.add(chkButton);
		
		//Create motion checkbox
		chkMotion = new JCheckBox("Motion Events");
		chkMotion.setMnemonic(KeyEvent.VK_M);
		chkMotion.addItemListener(this);
		chkMotion.setSelected(true);
		grpOptions.add(chkMotion);
		
		//Create feedback area
		txtEventInfo = new JTextArea();
		txtEventInfo.setEditable(false);
		txtEventInfo.setBackground(new Color(0xFF, 0xFF, 0xFF));
		txtEventInfo.setForeground(new Color(0x00, 0x00, 0x00));
		txtEventInfo.setText("");
		
		JScrollPane scrollPane = new JScrollPane(txtEventInfo);
		scrollPane.setPreferredSize(new Dimension(375, 125));
		add(scrollPane, BorderLayout.CENTER);
		
		setVisible(true);
	}
	

	/**
	 * @see java.awt.event.ItemListener#itemStateChanged(java.awt.event.ItemEvent)
	 */
	public void itemStateChanged(ItemEvent e) {
		ItemSelectable item = e.getItemSelectable();
		
        if (item == chkKeyboard) {
        	//Keyboard checkbox was changed, adjust listeners accordingly
        	if (e.getStateChange() == ItemEvent.SELECTED) {
        		GlobalScreen.getInstance().addNativeKeyListener(this);
        	}
        	else {
        		GlobalScreen.getInstance().removeNativeKeyListener(this);
        	}
		}
		else if (item == chkButton) {
			//Button checkbox was changed, adjust listeners accordingly
        	if (e.getStateChange() == ItemEvent.SELECTED) {
        		GlobalScreen.getInstance().addNativeMouseListener(this);
        	}
        	else {
        		GlobalScreen.getInstance().removeNativeMouseListener(this);
        	}
		}
		else if (item == chkMotion) {
			//Motion checkbox was changed, adjust listeners accordingly
        	if (e.getStateChange() == ItemEvent.SELECTED) {
        		GlobalScreen.getInstance().addNativeMouseMotionListener(this);
        	}
        	else {
        		GlobalScreen.getInstance().removeNativeMouseMotionListener(this);
        	}
		}
	}
	
	/**
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.
		txtEventInfo.setText("");
		
		//Return the focus to the window.
		this.requestFocusInWindow();	
	}
	
	/**
	 * @see org.jnativehook.keyboard.NativeKeyListener#keyPressed(org.jnativehook.keyboard.NativeKeyEvent)
	 */
	public void keyPressed(NativeKeyEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * @see org.jnativehook.keyboard.NativeKeyListener#keyReleased(org.jnativehook.keyboard.NativeKeyEvent)
	 */
	public void keyReleased(NativeKeyEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * @see org.jnativehook.mouse.NativeMouseListener#mousePressed(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void mousePressed(NativeMouseEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * @see org.jnativehook.mouse.NativeMouseListener#mouseReleased(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void mouseReleased(NativeMouseEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * @see org.jnativehook.mouse.NativeMouseMotionListener#mouseMoved(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void mouseMoved(NativeMouseEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * Write the <code>NativeKeyEvent</code> to the text window.
	 *
	 * @param e The native key event.
	 */
	private void displayEventInfo(NativeKeyEvent e) {
		txtEventInfo.append("\n" + e.paramString());
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
	}
	
	/**
	 * Write the <code>NativeMouseEvent</code> to the text window.
	 *
	 * @param e The native mouse event.
	 */
	private void displayEventInfo(NativeMouseEvent e) {
		txtEventInfo.append("\n" + e.paramString());
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
    }
	

	/**
	 * Unimplemented
	 * 
	 * @see java.awt.event.WindowListener#windowActivated(java.awt.event.WindowEvent)
	 */
	public void windowActivated(WindowEvent e) { /* Do Nothing */ }
	
	/**
	 * Unimplemented
	 *
	 * @see java.awt.event.WindowListener#windowClosing(java.awt.event.WindowEvent)
	 */
	public void windowClosing(WindowEvent e) {
		
	}
	
	/**
	 * Unimplemented
	 *
	 * @see java.awt.event.WindowListener#windowDeactivated(java.awt.event.WindowEvent)
	 */
	public void windowDeactivated(WindowEvent e) { /* Do Nothing */ }
	
	/**
	 * Unimplemented
	 *
	 * @see java.awt.event.WindowListener#windowDeiconified(java.awt.event.WindowEvent)
	 */
	public void windowDeiconified(WindowEvent e) { /* Do Nothing */ }
	
	/**
	 * Unimplemented
	 *
	 * @see java.awt.event.WindowListener#windowIconified(java.awt.event.WindowEvent)
	 */
	public void windowIconified(WindowEvent e) { /* Do Nothing */ }

	/**
	 * Write the auto repeat rate and delay to the text window along with any 
	 * errors that may have occurred.
	 *
	 * @see java.awt.event.WindowListener#windowOpened(java.awt.event.WindowEvent)
	 */
	public void windowOpened(WindowEvent e) {
		try {
			txtEventInfo.setText("Auto Repate Rate: " + GlobalScreen.getInstance().getAutoRepeatRate());
			txtEventInfo.append("\n" + "Auto Repate Delay: " + GlobalScreen.getInstance().getAutoRepeatDelay());
			txtEventInfo.append("\n" + "Double Click Time: " + GlobalScreen.getInstance().getDoubleClickTime());
			txtEventInfo.append("\n" + "Pointer Sensitivity: " + GlobalScreen.getInstance().getPointerSensitivity());
			txtEventInfo.append("\n" + "Pointer Acceleration Multiplier: " + GlobalScreen.getInstance().getPointerAccelerationMultiplier());
			txtEventInfo.append("\n" + "Pointer Acceleration Threshold: " + GlobalScreen.getInstance().getPointerAccelerationThreshold());
		}
		catch (Exception ex) {
			txtEventInfo.setText("Error: " + ex.toString() + "\n");
		}
		
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
	}
	
	/**
	 * Finalize and exit the program.
	 * 
	 * @see java.awt.event.WindowListener#windowClosed(java.awt.event.WindowEvent)
	 */
	public void windowClosed(WindowEvent e) {
		try {
			//Clean up the native hook.
			GlobalScreen.getInstance().unregisterNativeHook();
		}
		catch (NativeHookException ex) {
			ex.printStackTrace();
		}
		
		System.runFinalization();
		System.exit(0);
	}
	
    
	/**
	 * The demo project entry point.
	 *
	 * @param args unused.
	 */
	public static void main(String[] args) {
		new NativeHookDemo();
	}
}
