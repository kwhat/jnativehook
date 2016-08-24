/*
 
 SOME VK TO VC KEY CONVERSIONS
 
 LIST OF VK KEYS HERE: 
 https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
 
 
 MISSING FROM THIS TABLE:
 
 VC_POWER
 VC_WAKE
 VC_MEDIA_EJECT
 VC_APP_CALCULATOR
 VC_APP_MUSIC
 VC_APP_PICTURES
 VC_BROWSWER_HOME
 VC_UNDERSCORE
 VC_FURIGANA
 VC_KANJI
 VC_YEN
 ALL OF THE SUN KEYBOARD KEYS
 */

package org.jnativehook.keyboard;
import java.util.HashMap;
public final class keyConversion {
	public static final HashMap<Integer, Integer> map = new HashMap<Integer, Integer>(){
		{
			put(new Integer(0x07), 0x0000); // UNDEFINED
			put(new Integer(0x08), 0x000E); // Backspace
			put(new Integer(0x09), 0x000F); // Tab
			put(new Integer(0x0C), 0xE04C); // Clear 
			put(new Integer(0x0D), 0x001C); // Enter
			put(new Integer(0x12), 0x0038); // Alt Key
			put(new Integer(0x13), 0x0E45); // Pause
			put(new Integer(0x14), 0x003A); // Caps Lock
			put(new Integer(0x1B), 0x0001); // Escape
			put(new Integer(0x20), 0x0039); // SpaceBar
			put(new Integer(0x21), 0x0E49); // Page Up Key
			put(new Integer(0x22), 0x0E51); // Page Down Key
			put(new Integer(0x23), 0x0E4F); // End Key
			put(new Integer(0x24), 0x0E47); // Home Key
			put(new Integer(0x25), 0xE04B); // Left Arrow Key
			put(new Integer(0x26), 0xE048); // Up Arrow Key
			put(new Integer(0x27), 0xE04D); // Right Arrow Key
			put(new Integer(0x28), 0xE050); // Down Arrow Key
			put(new Integer(0x2C), 0x0E37); // Print Screen
			put(new Integer(0x2D), 0x0E52); // Insert
			put(new Integer(0x2E), 0x0E53); // Delete
			put(new Integer(0x30), 0x000B); // 0
			put(new Integer(0x31), 0x0002); // 1
			put(new Integer(0x32), 0x0003); // 2
			put(new Integer(0x33), 0x0004); // 3
			put(new Integer(0x34), 0x0005); // 4
			put(new Integer(0x35), 0x0006); // 5
			put(new Integer(0x36), 0x0007); // 6
			put(new Integer(0x37), 0x0008); // 7
			put(new Integer(0x38), 0x0009); // 8
			put(new Integer(0x39), 0x000A); // 9
			put(new Integer(0x41), 0x001E); // A
			put(new Integer(0x42), 0x0030); // B
			put(new Integer(0x43), 0x002E); // C
			put(new Integer(0x44), 0x0020); // D
			put(new Integer(0x45), 0x0012); // E
			put(new Integer(0x46), 0x0021); // F
			put(new Integer(0x47), 0x0022); // G
			put(new Integer(0x48), 0x0023); // H
			put(new Integer(0x49), 0x0017); // I
			put(new Integer(0x4A), 0x0024); // J
			put(new Integer(0x4B), 0x0025); // K
			put(new Integer(0x4C), 0x0026); // L
			put(new Integer(0x4D), 0x0032); // M
			put(new Integer(0x4E), 0x0031); // N
			put(new Integer(0x4F), 0x0018); // O
			put(new Integer(0x50), 0x0019); // P
			put(new Integer(0x51), 0x0010); // Q
			put(new Integer(0x52), 0x0013); // R
			put(new Integer(0x53), 0x001F); // S
			put(new Integer(0x54), 0x0014); // T
			put(new Integer(0x55), 0x0016); // U
			put(new Integer(0x56), 0x002F); // V
			put(new Integer(0x57), 0x0011); // W
			put(new Integer(0x58), 0x002D); // X
			put(new Integer(0x59), 0x0015); // Y
			put(new Integer(0x5A), 0x002C); // Z
			put(new Integer(0x5B), 0x0E5B); // Windows
			put(new Integer(0x5C), 0x0E5C); // Windows
			put(new Integer(0x5D), 0x0E5D); // Context Menu
			put(new Integer(0x5F), 0xE05F); // Sleep
			put(new Integer(0x60), 0x0052); // KP 0
			put(new Integer(0x61), 0x004F); // KP 1
			put(new Integer(0x62), 0x0050); // KP 2
			put(new Integer(0x63), 0x0051); // KP 3
			put(new Integer(0x64), 0x004B); // KP 4
			put(new Integer(0x65), 0x004C); // KP 5
			put(new Integer(0x66), 0x004D); // KP 6
			put(new Integer(0x67), 0x0047); // KP 7
			put(new Integer(0x68), 0x0048); // KP 8
			put(new Integer(0x69), 0x0049); // KP 9
			put(new Integer(0x6A), 0x0037); // KP Multiply
			put(new Integer(0x6B), 0x004E); // KP Add
			put(new Integer(0x6C), 0x0053); // KP Separator
			put(new Integer(0x6D), 0x004A); // KP Subtract
			put(new Integer(0x6F), 0x0E35); // KP Divide
			put(new Integer(0x70), 0x003B); // F1
			put(new Integer(0x71), 0x003C); // F2
			put(new Integer(0x72), 0x003D); // F3
			put(new Integer(0x73), 0x003E); // F4
			put(new Integer(0x74), 0x003F); // F5
			put(new Integer(0x75), 0x0040); // F6
			put(new Integer(0x76), 0x0041); // F7
			put(new Integer(0x77), 0x0042); // F8
			put(new Integer(0x78), 0x0043); // F9
			put(new Integer(0x79), 0x0044); // F10
			put(new Integer(0x7A), 0x0057); // F11
			put(new Integer(0x7B), 0x0058); // F12
			put(new Integer(0x7C), 0x005B); // F13
			put(new Integer(0x7D), 0x005C); // F14
			put(new Integer(0x7E), 0x005D); // F15
			put(new Integer(0x7F), 0x0063); // F16
			put(new Integer(0x80), 0x0064); // F17
			put(new Integer(0x81), 0x0065); // F18
			put(new Integer(0x82), 0x0066); // F19
			put(new Integer(0x83), 0x0067); // F20
			put(new Integer(0x84), 0x0068); // F21
			put(new Integer(0x85), 0x0069); // F22
			put(new Integer(0x86), 0x006A); // F23
			put(new Integer(0x87), 0x006B); // F24
			put(new Integer(0x90), 0x0045); // NUM LOCK
			put(new Integer(0x91), 0x0046); // SCROLL LOCK
			put(new Integer(0xA0), 0x002A); // LSHIFT
			put(new Integer(0xA1), 0x0036); // RSHIFT
			put(new Integer(0xA2), 0x001D); // LCONTROL
			put(new Integer(0xA3), 0x0E1D); // RCONTROL
			put(new Integer(0xA6), 0xE06A); // Browser Back
			put(new Integer(0xA7), 0xE069); // Browser Forward
			put(new Integer(0xA8), 0xE067); // Browser Refresh
			put(new Integer(0xA9), 0xE068); // Browser Stop
			put(new Integer(0xAA), 0xE065); // Browser Search
			put(new Integer(0xAB), 0xE066); // Browser Favorites
			put(new Integer(0xAD), 0xE020); // Volume Mute
			put(new Integer(0xAE), 0xE02E); // Volume Down
			put(new Integer(0xAF), 0xE030); // Volume Up
			put(new Integer(0xB0), 0xE019); // Media Next
			put(new Integer(0xB1), 0xE010); // Previous Track
			put(new Integer(0xB2), 0xE024); // Media Stop
			put(new Integer(0xB3), 0xE022); // Media Play/Pause
			put(new Integer(0xB4), 0xE06C); // Start Mail
			put(new Integer(0xB5), 0xE06D); // Media Select
			put(new Integer(0xBA), 0x0027); // ; key
			put(new Integer(0xBB), 0x000D); // = key
			put(new Integer(0xBC), 0x0033); // , key
			put(new Integer(0xBD), 0x000C); // - key
			put(new Integer(0xBE), 0x0034); // . key
			put(new Integer(0xBF), 0x0035); // / key
			put(new Integer(0xC0), 0x0029); // Back Quote
			put(new Integer(0xDB), 0x001A); // [
			put(new Integer(0xDD), 0x001B); // ]
			put(new Integer(0xDC), 0x002B); // Backslash
			put(new Integer(0xDE), 0x0028); // Quotes
            put(new Integer(0x0f1), 0x0070); // KATAKANA
            put(new Integer(0x0f2), 0x007B); // Hiragana
		}	

	};
}
