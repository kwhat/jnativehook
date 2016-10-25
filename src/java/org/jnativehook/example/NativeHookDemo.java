/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.example;

// Imports.
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import org.jnativehook.NativeInputEvent;
import org.jnativehook.SwingDispatchService;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseInputListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.text.BadLocationException;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.ItemSelectable;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Date;
import java.util.logging.ConsoleHandler;
import java.util.logging.Formatter;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

/**
 * A demonstration of how to use the JNativeHook library.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.0
 * @since	1.0
 *
 * @see GlobalScreen
 * @see NativeKeyListener
 */
public class NativeHookDemo extends JFrame implements ActionListener, ItemListener, NativeKeyListener, NativeMouseInputListener, NativeMouseWheelListener, WindowListener {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 1541183202160543102L;

	/** Menu Items */
	private JMenu menuSubListeners;
	private JMenuItem menuItemQuit, menuItemClear;
	private JCheckBoxMenuItem menuItemEnable, menuItemKeyboardEvents, menuItemButtonEvents, menuItemMotionEvents, menuItemWheelEvents;

	/** The text area to display event info. */
	private JTextArea txtEventInfo;

	/** Logging */
	private static final Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());

	/**
	 * Instantiates a new native hook demo.
	 */
	public NativeHookDemo() {
		// Setup the main window.
		setTitle("JNativeHook Demo");
		setLayout(new BorderLayout());
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		setSize(600, 300);
		addWindowListener(this);

		JMenuBar menuBar = new JMenuBar();

		// Create the file menu.
		JMenu menuFile = new JMenu("File");
		menuFile.setMnemonic(KeyEvent.VK_F);
		menuBar.add(menuFile);

		menuItemQuit = new JMenuItem("Quit", KeyEvent.VK_Q);
		menuItemQuit.addActionListener(this);
		menuItemQuit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F4, ActionEvent.ALT_MASK));
		menuItemQuit.getAccessibleContext().setAccessibleDescription("Exit the program");
		menuFile.add(menuItemQuit);

		// Create the view.
		JMenu menuView = new JMenu("View");
		menuView.setMnemonic(KeyEvent.VK_V);
		menuBar.add(menuView);

		menuItemClear = new JMenuItem("Clear", KeyEvent.VK_C);
		menuItemClear.addActionListener(this);
		menuItemClear.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuItemClear.getAccessibleContext().setAccessibleDescription("Clear the screen");
		menuView.add(menuItemClear);

		menuView.addSeparator();

		menuItemEnable = new JCheckBoxMenuItem("Enable Native Hook");
		menuItemEnable.addItemListener(this);
		menuItemEnable.setMnemonic(KeyEvent.VK_H);
		menuItemEnable.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuView.add(menuItemEnable);

		// Create the listeners sub menu.
		menuSubListeners = new JMenu("Listeners");
		menuSubListeners.setMnemonic(KeyEvent.VK_L);
		menuView.add(menuSubListeners);

		menuItemKeyboardEvents = new JCheckBoxMenuItem("Keyboard Events");
		menuItemKeyboardEvents.addItemListener(this);
		menuItemKeyboardEvents.setMnemonic(KeyEvent.VK_K);
		menuItemKeyboardEvents.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_K, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuSubListeners.add(menuItemKeyboardEvents);

		menuItemButtonEvents = new JCheckBoxMenuItem("Button Events");
		menuItemButtonEvents.addItemListener(this);
		menuItemButtonEvents.setMnemonic(KeyEvent.VK_B);
		menuItemButtonEvents.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_B, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuSubListeners.add(menuItemButtonEvents);

		menuItemMotionEvents = new JCheckBoxMenuItem("Motion Events");
		menuItemMotionEvents.addItemListener(this);
		menuItemMotionEvents.setMnemonic(KeyEvent.VK_M);
		menuItemMotionEvents.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_M, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuSubListeners.add(menuItemMotionEvents);

		menuItemWheelEvents = new JCheckBoxMenuItem("Wheel Events");
		menuItemWheelEvents.addItemListener(this);
		menuItemWheelEvents.setMnemonic(KeyEvent.VK_W);
		menuItemWheelEvents.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_W, ActionEvent.CTRL_MASK + ActionEvent.SHIFT_MASK));
		menuSubListeners.add(menuItemWheelEvents);

		setJMenuBar(menuBar);

		// Create feedback area.
		txtEventInfo = new JTextArea();
		txtEventInfo.setEditable(false);
		txtEventInfo.setBackground(new Color(0xFF, 0xFF, 0xFF));
		txtEventInfo.setForeground(new Color(0x00, 0x00, 0x00));
		txtEventInfo.setText("");

		JScrollPane scrollPane = new JScrollPane(txtEventInfo);
		scrollPane.setPreferredSize(new Dimension(375, 125));
		add(scrollPane, BorderLayout.CENTER);


		// Disable parent logger and set the desired level.
		logger.setUseParentHandlers(false);
		logger.setLevel(Level.ALL);

		// Add our custom formatter to a console handler.
		ConsoleHandler handler = new ConsoleHandler();
		handler.setFormatter(new LogFormatter());
		handler.setLevel(Level.WARNING);
		logger.addHandler(handler);

		/* Note: JNativeHook does *NOT* operate on the event dispatching thread.
		 * Because Swing components must be accessed on the event dispatching
		 * thread, you *MUST* wrap access to Swing components using the
		 * SwingUtilities.invokeLater() or EventQueue.invokeLater() methods.
		 */
		GlobalScreen.setEventDispatcher(new SwingDispatchService());
		
		setVisible(true);
	}

	/**
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == menuItemQuit) {
			this.dispose();
		}
		else if (e.getSource() == menuItemClear) {
			txtEventInfo.setText("");
		}
	}

	/**
	 * @see java.awt.event.ItemListener#itemStateChanged(java.awt.event.ItemEvent)
	 */
	public void itemStateChanged(ItemEvent e) {
		ItemSelectable item = e.getItemSelectable();

		if (item == menuItemEnable) {
			try {
				// Keyboard checkbox was changed, adjust listeners accordingly.
				if (e.getStateChange() == ItemEvent.SELECTED) {
					// Initialize native hook.  This is done on window open because the
					// listener requires the txtEventInfo object to be constructed.
					GlobalScreen.registerNativeHook();
				}
				else {
					GlobalScreen.unregisterNativeHook();
				}
			}
			catch (NativeHookException ex) {
				txtEventInfo.append("Error: " + ex.getMessage() + "\n");
			}

			// Set the enable menu item to the state of the hook.
			menuItemEnable.setState(GlobalScreen.isNativeHookRegistered());

			// Set enable/disable the sub-menus based on the enable menu item's state.
			menuSubListeners.setEnabled(menuItemEnable.getState());
		}
		else if (item == menuItemKeyboardEvents) {
			// Keyboard checkbox was changed, adjust listeners accordingly
			if (e.getStateChange() == ItemEvent.SELECTED) {
				GlobalScreen.addNativeKeyListener(this);
			}
			else {
				GlobalScreen.removeNativeKeyListener(this);
			}
		}
		else if (item == menuItemButtonEvents) {
			// Button checkbox was changed, adjust listeners accordingly
			if (e.getStateChange() == ItemEvent.SELECTED) {
				GlobalScreen.addNativeMouseListener(this);
			}
			else {
				GlobalScreen.removeNativeMouseListener(this);
			}
		}
		else if (item == menuItemMotionEvents) {
			// Motion checkbox was changed, adjust listeners accordingly
			if (e.getStateChange() == ItemEvent.SELECTED) {
				GlobalScreen.addNativeMouseMotionListener(this);
			}
			else {
				GlobalScreen.removeNativeMouseMotionListener(this);
			}
		}
		else if (item == menuItemWheelEvents) {
			// Motion checkbox was changed, adjust listeners accordingly
			if (e.getStateChange() == ItemEvent.SELECTED) {
				GlobalScreen.addNativeMouseWheelListener(this);
			}
			else {
				GlobalScreen.removeNativeMouseWheelListener(this);
			}
		}
	}

	/**
	 * @see org.jnativehook.keyboard.NativeKeyListener#nativeKeyPressed(org.jnativehook.keyboard.NativeKeyEvent)
	 */
	public void nativeKeyPressed(NativeKeyEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.keyboard.NativeKeyListener#nativeKeyReleased(org.jnativehook.keyboard.NativeKeyEvent)
	 */
	public void nativeKeyReleased(NativeKeyEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.keyboard.NativeKeyListener#nativeKeyTyped(org.jnativehook.keyboard.NativeKeyEvent)
	 */
	public void nativeKeyTyped(NativeKeyEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseListener#nativeMouseClicked(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void nativeMouseClicked(NativeMouseEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseListener#nativeMousePressed(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void nativeMousePressed(NativeMouseEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseListener#nativeMouseReleased(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void nativeMouseReleased(NativeMouseEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseMotionListener#nativeMouseMoved(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void nativeMouseMoved(NativeMouseEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseMotionListener#nativeMouseDragged(org.jnativehook.mouse.NativeMouseEvent)
	 */
	public void nativeMouseDragged(NativeMouseEvent e) {
		displayEventInfo(e);
	}

	/**
	 * @see org.jnativehook.mouse.NativeMouseWheelListener#nativeMouseWheelMoved(org.jnativehook.mouse.NativeMouseWheelEvent)
	 */
	public void nativeMouseWheelMoved(NativeMouseWheelEvent e) {
		displayEventInfo(e);
	}

	/**
	 * Write information about the <code>NativeInputEvent</code> to the text
	 * window.
	 *
	 * @param e the native input event to display.
	 */
	private void displayEventInfo(final NativeInputEvent e) {
		txtEventInfo.append("\n" + e.paramString());

		try {
			//Clean up the history to reduce memory consumption.
			if (txtEventInfo.getLineCount() > 100) {
				txtEventInfo.replaceRange("", 0, txtEventInfo.getLineEndOffset(txtEventInfo.getLineCount() - 1 - 100));
			}

			txtEventInfo.setCaretPosition(txtEventInfo.getLineStartOffset(txtEventInfo.getLineCount() - 1));
		}
		catch (BadLocationException ex) {
			txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
		}
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
	public void windowClosing(WindowEvent e) { /* Do Nothing */ }

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
	 * Display information about the native keyboard and mouse along with any
	 * errors that may have occurred.
	 *
	 * @see java.awt.event.WindowListener#windowOpened(java.awt.event.WindowEvent)
	 */
	public void windowOpened(WindowEvent e) {
		// Return the focus to the window.
		this.requestFocusInWindow();

		// Enable the hook, this will cause the GlobalScreen to be initilized.
		menuItemEnable.setSelected(true);

		// Please note that these properties are not available until after the GlobalScreen class is initialized.
		txtEventInfo.append("JNativeHook Version " + System.getProperty("jnativehook.lib.version"));
		txtEventInfo.append("\nAuto Repeat Rate: " + System.getProperty("jnativehook.key.repeat.rate"));
		txtEventInfo.append("\n" + "Auto Repeat Delay: " + System.getProperty("jnativehook.key.repeat.delay"));
		txtEventInfo.append("\n" + "Double Click Time: " + System.getProperty("jnativehook.button.multiclick.iterval"));
		txtEventInfo.append("\n" + "Pointer Sensitivity: " + System.getProperty("jnativehook.pointer.sensitivity"));
		txtEventInfo.append("\n" + "Pointer Acceleration Multiplier: " + System.getProperty("jnativehook.pointer.acceleration.multiplier"));
		txtEventInfo.append("\n" + "Pointer Acceleration Threshold: " + System.getProperty("jnativehook.pointer.acceleration.threshold"));

		try {
			txtEventInfo.setCaretPosition(txtEventInfo.getLineStartOffset(txtEventInfo.getLineCount() - 1));
		}
		catch (BadLocationException ex) {
			txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
		}

		// Enable all of the listeners.
		menuItemKeyboardEvents.setSelected(true);
		menuItemButtonEvents.setSelected(true);
		menuItemMotionEvents.setSelected(true);
		menuItemWheelEvents.setSelected(true);
	}

	/**
	 * Finalize and exit the program.
	 *
	 * @see java.awt.event.WindowListener#windowClosed(java.awt.event.WindowEvent)
	 */
	public void windowClosed(WindowEvent e) {
		// Clean up the native hook.
		try {
			GlobalScreen.unregisterNativeHook();
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
		StringBuffer copyright = new StringBuffer("\n")
				.append("JNativeHook: Global keyboard and mouse hooking for Java.\n")
				.append("Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.\n")
				.append("https://github.com/kwhat/jnativehook/\n")
				.append("\n")
				.append("JNativeHook is free software: you can redistribute it and/or modify\n")
				.append("it under the terms of the GNU Lesser General Public License as published\n")
				.append("by the Free Software Foundation, either version 3 of the License, or\n")
				.append("(at your option) any later version.\n")
				.append("\n")
				.append("JNativeHook is distributed in the hope that it will be useful,\n")
				.append("but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
				.append("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
				.append("GNU General Public License for more details.\n")
				.append("\n")
				.append("You should have received a copy of the GNU Lesser General Public License\n")
				.append("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
		System.out.println(copyright);

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new NativeHookDemo();
			}
		});
	}

	/**
	 * A simple log formatter.
	 *
	 * @see java.util.Formatter
	 */
	private final class LogFormatter extends Formatter {
		@Override
		public String format(LogRecord record) {
			StringBuilder line = new StringBuilder();

			line.append(new Date(record.getMillis()))
				.append(" ")
				.append(record.getLevel().getLocalizedName())
				.append(":\t")
				.append(formatMessage(record));

			if (record.getThrown() != null) {
				try {
					StringWriter sw = new StringWriter();
					PrintWriter pw = new PrintWriter(sw);
					record.getThrown().printStackTrace(pw);
					pw.close();
					line.append(sw.toString());
					sw.close();
				}
				catch (Exception ex) { /* Do Nothing */ }
			}

			return line.toString();
		}
	}
}
