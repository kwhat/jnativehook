package org.jnativehook.example;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.*;
import javax.swing.*;

public class KeyEventDemo extends JFrame implements KeyListener, ActionListener, FocusListener {
	boolean oneEvent = false;
	
    JTextArea displayArea;
    JTextField typingArea;
    
    JButton clearButton;
    JButton enableButton;
    
    
    public KeyEventDemo() {
        setTitle("JNativeHook Demo");
        setLayout(new GridBagLayout());
        
        typingArea = new JTextField();
        typingArea.addKeyListener(this);
        
        //Uncomment this if you wish to turn off focus
        //traversal.  The focus subsystem consumes
        //focus traversal keys, such as Tab and Shift Tab.
        //If you uncomment the following line of code, this
        //disables focus traversal and the Tab events will
        //become available to the key event listener.
        typingArea.setFocusTraversalKeysEnabled(false);
        
        displayArea = new JTextArea();
        displayArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(displayArea);
        scrollPane.setPreferredSize(new Dimension(375, 125));
        
        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(5, 5, 5, 5);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;
        
        add(typingArea, c);
        add(scrollPane, c);
        
    }

    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    private static void createAndShowGUI() {
        //Create and set up the window.
        KeyEventDemo frame = new KeyEventDemo("KeyEventDemo");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        //Set up the content pane.
        frame.addComponentsToPane();
        
        
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }
    

    
    
    /** Handle the key typed event from the text field. */
    public void keyTyped(KeyEvent e) {
    	if (!oneEvent) 
    	{
    		displayInfo(e, "KEY TYPED: ");
    		oneEvent =true;
    	}
    }
    
    /** Handle the key pressed event from the text field. */
    public void keyPressed(KeyEvent e) {
    	if (!oneEvent)
    	{
    		displayInfo(e, "KEY PRESSED: ");
    		oneEvent =true;
    	}
    }
    
    /** Handle the key released event from the text field. */
    public void keyReleased(KeyEvent e) {
    	if (!oneEvent)
    	{
    		displayInfo(e, "KEY RELEASED: ");
    		oneEvent =true;
    	}
    }
    
    /** Handle the button click. */
    public void actionPerformed(ActionEvent e) {
        //Clear the text components.
    	
    	/*
    	System.out.println(e.getID());
    	System.out.println(e.getSource());
    	System.out.println(e.getActionCommand());
    	
    	System.out.println();
    	*/
    	
    	if (e.getActionCommand() == "Enable")
    	{
    		oneEvent =false;
    	}
    	else
    	{
    		displayArea.setText("");
        	typingArea.setText("");
        
        //	Return the focus to the typing area.
        	typingArea.requestFocusInWindow();
    	}
    }
    
    /*
     * We have to jump through some hoops to avoid
     * trying to print non-printing characters
     * such as Shift.  (Not only do they not print,
     * but if you put them in a String, the characters
     * afterward won't show up in the text area.)
     */
    private void displayInfo(KeyEvent e, String keyStatus){
        
        //You should only rely on the key char if the event
        //is a key typed event.
        int id = e.getID();
        String keyString;
        if (id == KeyEvent.KEY_TYPED) {
            char c = e.getKeyChar();
            keyString = "key character = '" + c + "'";
        } else {
            int keyCode = e.getKeyCode();
            keyString = "key code = " + keyCode
                    + " ("
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
        } else {
            actionString += "NO";
        }
        
        String locationString = "key location: ";
        int location = e.getKeyLocation();
        if (location == KeyEvent.KEY_LOCATION_STANDARD) {
            locationString += "standard";
        } else if (location == KeyEvent.KEY_LOCATION_LEFT) {
            locationString += "left";
        } else if (location == KeyEvent.KEY_LOCATION_RIGHT) {
            locationString += "right";
        } else if (location == KeyEvent.KEY_LOCATION_NUMPAD) {
            locationString += "numpad";
        } else { // (location == KeyEvent.KEY_LOCATION_UNKNOWN)
            locationString += "unknown";
        }
        
        displayArea.append(keyStatus + newline
                + "    " + keyString + newline
                + "    " + modString + newline
                + "    " + actionString + newline
                + "    " + locationString + newline);
        displayArea.setCaretPosition(displayArea.getDocument().getLength());
    }
    
    public static void main(String[] args) {
        /* Turn off metal's use of bold fonts */
        UIManager.put("swing.boldMetal", Boolean.FALSE);
        
        //Schedule a job for event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }   
}
