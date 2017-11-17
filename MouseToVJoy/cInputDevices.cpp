#include "input.h"

void cInputDevices::GetData(LPARAM lParam)
{
	// Determine how big the buffer should be
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));

	// Create a buffer of the correct size - but see note below

	// Call the function again, this time with the buffer to get the data
	if (bufferSize <= 40)
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)m_buffer, &bufferSize, sizeof(RAWINPUTHEADER));

	RAWINPUT *raw = (RAWINPUT*)m_buffer;

	// The mouse has not been tested extensively,
	// but I believe it works.
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		// Get values from the mouse member (of type RAWMOUSE)
		m_nMouseXChange = raw->data.mouse.lLastX;
		m_nMouseYChange = raw->data.mouse.lLastY;



		bool bStateDown = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN;
		bool bStateUp = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP;

		if (bStateDown == true && bStateUp == false)
		{
			m_bLMB = true;
		}

		if (bStateUp == true)
		{
			m_bLMB = false;
		}


		bool bStateDownTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN;
		bool bStateUpTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP;

		if (bStateDownTwo == true && bStateUpTwo == false)
		{
			m_bRMB = true;
		}

		if (bStateUpTwo == true)
		{
			m_bRMB = false;
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
				pbToKey = &m_baKeyboard[i+0x01];

				// At this point we have assigned our boolean pointer variable
				// a new address which is whatever index i would be accessing.
				// We break because there is no need to go further since we found the
				// matching keycode!
				break;
			}
		}

		if (pbToKey != NULL)
		{

			*pbToKey = CheckKeyPress(*pbToKey, keyUp);

			// Be sure to return ASAP!
			return;
		}

		// TODO: Add more key ranges to check for! Just follow
		// how I have set things up so far.

	}


}

bool cInputDevices::CheckKeyPress(bool bLastKeyState, bool bThisKeyState)
{
	// The following may be overkill, but just know the value returned
	// is based upon the current state of the boolean in question, and the
	// new state we have processed.
	if (bThisKeyState == false)
	{
		if (bLastKeyState == true)
		{
			return true;
		}
		else
		{
			return true;
		}
	}
	else if (bThisKeyState == true)
	{
		if (bLastKeyState == false)
		{
			return false;
		}
		else
			return false;
	}
}
