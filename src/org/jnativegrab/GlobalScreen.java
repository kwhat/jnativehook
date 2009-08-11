package org.jnativegrab;

//Imports
import java.awt.Component;

public class GlobalScreen extends Component {
	private static final long serialVersionUID = -3980582715467809283L;
	private static GlobalScreen instance = null;
	
	public GlobalScreen() {
		//Do Nothing
	}
	
	public static GlobalScreen getInstance() {
		if (GlobalScreen.instance == null) {
			GlobalScreen.instance = new GlobalScreen();
		}
		
		return GlobalScreen.instance;
	}
}
