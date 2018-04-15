#ifndef _INPUT_H
#define _INPUT_H

// This code was built based upon what I learned from an internet site called ToyMaker.
// In my opinion, their site is a great source for most of the basics one needs to know 
// to get started in game programming. Mainpage: http://www.toymaker.info/

// Yes this is for WINDOWS only.
#include <Windows.h>

// The follwoing enumerations are set to cover related key-ranges from
// A-Z and left, up, right, down.
// Source page: 
// http://msdn.microsoft.com/en-us/library/ms645540%28VS.85%29.aspx

class CInputDevices
{
public:
	// The constructor is important. I suppose I could of made a separate function for initalization
	// but I am trying to keep a consistent attitude with things.


	// Only call this once when your Application begins.
	// I may be wrong, but I saw nothing from what I read that indicated
	// devices need to be unregistered.


	// Everytime your MsgProc's uMsg is WM_INPUT, you will want to call the GetData function.
	// I believe this is smarter than calling it every frame in a game loop.

	void getData(LPARAM lParam);

	// This should be self-explanatory. Depending on which function
	// you call, you will get true or false about the state of the mouse button.
	// The code in the source file may be a bit difficult to follow,
	// but I tried my best to make it match what was described as the best way
	// to be sure the 

	bool           isLeftMouseButtonDown() { return _isLeftMouseButtonPressed; }
	bool           isRightMouseButtonDown() { return _isRightMouseButtonPressed; }

	// These functions return values that are relative to the change in position
	// of your mouse.

	int            getMouseChangeX() { return _mouseXChange; }
	int            getMouseChangeY() { return _mouseYChange; }

	// OPTIONALLY:
	// To obtain exact mouse coords check the uMsg in your Application's MsgProc
	// for WM_MOUSEMOVE, and use HIWORD() LOWORD() functions to extract the mouse X,Y
	// from lParam. Store them in the below variables.

	int            mouseX;
	int            mouseY;

	// Alphabetic as in any letter from the Alphabet. So IsAlphabeticKeyDown returns
	// a value of true or false from an array of 25 booleans. Each index is associated
	// with a position in the english alphabet. Use one of the enumerated values
	// such as VKey_A and subtract 0x41 or make a different enumeration list
	// for all 25 letters and set the first value to 0.

	bool           isAlphabeticKeyDown(int letter) { return _isKeyboardButtonPressed[letter]; }

private:

	// Two input devices are covered by this class. Mouse and Keyboard.
	// A good reference for them is: http://www.toymaker.info/Games/html/raw_input.html
	RAWINPUTDEVICE _rawInputDevice[2];

	// As said on www.toymaker.info the mouse size is 40 and the keyboard 32.
	// So it appears as if the buffer will be filled with either or, not both.
	// Thus, just 40 bytes.
	BYTE           _buffer[40];

	// Should be obvious what functions return these two values.
	int            _mouseXChange;
	int            _mouseYChange;

	// Again, should be obvious what functions use these. 
	// LMB = Left Mouse Button. RMB = Right Mouse Button
	bool           _isLeftMouseButtonPressed;
	bool           _isRightMouseButtonPressed;

	// Notice how I use an array of bools rather than having a separate bool for each.
	// In the source file you will see how my enumerations come into play for knowing what index
	// to access.
	bool           _isKeyboardButtonPressed[166];

	// The CheckKeyPress function is because of these issues:

	// 1. The Mouse
	// The flag is only set once when the button is pressed and so to determine if the mouse button is being held down you need to record it as down
	// until you get a RI_MOUSE_LEFT_BUTTON_UP flag.

	// 2. The Keyboard
	// When a key is held down the raw->data.heyboard.Flags has the RI_KEY_MAKE  bit set (actually none as the value is 0) and when the key is released 
	// the  RI_KEY_BREAK  bit is set (value is 1). 

	// 1 and 2 is what I got directly from ToyMaker's website. I believe for the most part CheckKeyPress takes what was said into account.
	bool           checkKeyPress(bool bLastKeyState, bool bThisKeyState);

};

#endif