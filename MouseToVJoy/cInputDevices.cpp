#include "input.h"

void CInputDevices::getData(LPARAM lParam)
{
	// Determine how big the buffer should be
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));

	// Create a buffer of the correct size - but see note below

	// Call the function again, this time with the buffer to get the data
	if (bufferSize <= 40)
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)_buffer, &bufferSize, sizeof(RAWINPUTHEADER));

	RAWINPUT *raw = (RAWINPUT*)_buffer;

	// The mouse has not been tested extensively,
	// but I believe it works.
	_mouseXChange = raw->data.mouse.lLastX;
	_mouseYChange = raw->data.mouse.lLastY;
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		// Get values from the mouse member (of type RAWMOUSE)
		bool bStateDown = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN;
		bool bStateUp = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP;

		if (bStateDown == true && bStateUp == false)
		{
			_isLeftMouseButtonPressed = true;
			_isKeyboardButtonPressed[0x01] = true;
		}

		if (bStateUp == true)
		{
			_isLeftMouseButtonPressed = false;
			_isKeyboardButtonPressed[0x01] = false;
		}


		bool bStateDownTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN;
		bool bStateUpTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP;

		if (bStateDownTwo == true && bStateUpTwo == false)
		{
			_isRightMouseButtonPressed = true;
			_isKeyboardButtonPressed[0x02] = true;
			
		}

		if (bStateUpTwo == true)
		{
			_isRightMouseButtonPressed = false;
			_isKeyboardButtonPressed[0x02] = false;
		}


	}
	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		// Get key value from the keyboard member (of type RAWKEYBOARD)
		USHORT keyCode = raw->data.keyboard.VKey;
		bool keyUp = raw->data.keyboard.Flags & RI_KEY_BREAK;

		// Ok because I put the most effort into this section of my code,
		// I will briefly explain.

		// We need a boolean pointer to start.
		bool* pbToKey = NULL;

		// The keycode can be one of many possibilities,
		// which is why a switch case is to exhaustive to
		// use and I believe for the most part switch case 
		// would not be efficient.

		// So instead of manually typing every possible
		// case value, we can just start by looping through
		// an expected range of keys the keycode might be.

		for (int i = 0; i < 165; ++i)
		{
			// We add the hex-code 0x41 because that is the
			// value MSDN lists for virtual keycode 'A'.
			if (keyCode == i + 0x01)
			{
				// However our alphabet or array of booleans that
				// represent it, begins at 0 and is only 25 in length.
				// So i itself is the appropritate index.
				pbToKey = &_isKeyboardButtonPressed[i+0x01];

				// At this point we have assigned our boolean pointer variable
				// a new address which is whatever index i would be accessing.
				// We break because there is no need to go further since we found the
				// matching keycode!
				break;
			}
		}

		if (pbToKey != NULL)
		{

			*pbToKey = checkKeyPress(*pbToKey, keyUp);

			// Be sure to return ASAP!
			return;
		}

		// TODO: Add more key ranges to check for! Just follow
		// how I have set things up so far.

	}
}

bool CInputDevices::checkKeyPress(bool isLastKeyState, bool isThisKeyState)
{
	// The following may be overkill, but just know the value returned
	// is based upon the current state of the boolean in question, and the
	// new state we have processed.
	if (isThisKeyState == false)
	{
		if (isLastKeyState == true)
		{
			return true;
		}
		else
		{
			return true;
		}
	}
	else if (isThisKeyState == true)
	{
		if (isLastKeyState == false)
		{
			return false;
		}
		else
			return false;
	}
}
