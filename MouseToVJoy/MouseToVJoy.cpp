#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "input.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "public.h"
#include "Math.h"
#include <tchar.h>
#include "vJoyInput.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#define DEV_ID 1
DOUBLE VerApp = 1.1;

const char g_szClassName[] = "myWindowClass";
HWND hwnd;
WNDCLASSEX wc;
MSG Msg;
cInputDevices rInput;
vJoyInput vJ;
BOOL CursorLocked = false;
//Vjoy
VjdStat status;
JOYSTICK_POSITION_V2 iReport;
UINT DevID = DEV_ID;
INT X = 0;
INT Y = 0;
INT Z = 0;
INT RX = 0;
PVOID pPositionMessage;
UINT	IoCode = LOAD_POSITIONS;
UINT	IoSize = sizeof(JOYSTICK_POSITION);
// HID_DEVICE_ATTRIBUTES attrib;
UINT iInterface = 1;
DOUBLE Sensitivity, AttackTimeThrottle, ReleaseTimeThrottle, AttackTimeBreak, ReleaseTimeBreak, AttackTimeClutch, ReleaseTimeClutch, ThrottleKey, BreakKey, ClutchKey, MouseLockKey, UseMouse;

//Vjoy
// Step 4: the Window Procedure


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

		for (int iii = 0; iii < 25; ++iii)
		{
			// We add the hex-code 0x41 because that is the
			// value MSDN lists for virtual keycode 'A'.
			if (keyCode == iii + 0x41)
			{
				// However our alphabet or array of booleans that
				// represent it, begins at 0 and is only 25 in length.
				// So iii itself is the appropritate index.
				pbToKey = &m_baAlphabet[iii];

				// At this point we have assigned our boolean pointer variable
				// a new address which is whatever index iii would be accessing.
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

int vJoyInput::TestDriver() {

		_tprintf("Assetto Corsa Mouse to vJoy Feeder\n");
		_tprintf("==================================\n");
		_tprintf("Author: R1per\n");
		_tprintf("Version: %.1f\n", VerApp);
		// Get the driver attributes (Vendor ID, Product ID, Version Number)
		if (!vJoyEnabled())
		{
			_tprintf("Failed Getting vJoy attributes.\n");
			return -2;
		}
		else
		{
			_tprintf("vJoy Version Number: %S\n",TEXT(GetvJoySerialNumberString()));
		};
		// Test interface DLL matches vJoy driver
		// Compare versions
		WORD VerDll, VerDrv;
		if (!DriverMatch(&VerDll, &VerDrv))
			_tprintf("Failed\r\nvJoy Driver (version %04x) does not match\
 vJoyInterface DLL (version %04x)\n", VerDrv, VerDll);
		else
			_tprintf("vJoyInterface DLL Version: %04x\n", VerDrv);
			_tprintf("OK - Driver and DLL match\n", VerDrv);


	}

int vJoyInput::TestVirtualDevices() {
		// Get the state of the requested device (iInterface)
		status = GetVJDStatus(iInterface);
		switch (status)
		{
		case VJD_STAT_OWN:
			_tprintf("vJoy Device %d is already owned by this feeder\n", iInterface);
			break;
		case VJD_STAT_FREE:
			_tprintf("vJoy Device %d is free\n", iInterface);
			break;
		case VJD_STAT_BUSY:
			_tprintf("vJoy Device %d is already owned by another feeder\n\
Cannot continue\n", iInterface);
			return -3;
		case VJD_STAT_MISS:
			_tprintf("vJoy Device %d is not installed or disabled\n\
Cannot continue\n", iInterface);
			return -4;
		default:
			_tprintf("vJoy Device %d general error\nCannot continue\n", iInterface);
			return -1;
		};
	}

int vJoyInput::AccuireDevice() {
		// Acquire the target if not already owned
		if ((status == VJD_STAT_OWN) || \
			((status == VJD_STAT_FREE) && (!AcquireVJD(iInterface))))
		{
			_tprintf("Failed to acquire vJoy device number %d.\n", iInterface);
			return -1;
		}
		else
			_tprintf("Acquired: vJoy device number %d.\n", iInterface);
	}

void vJoyInput::FeedDevice() {
			iReport.bDevice = iInterface;
			iReport.wAxisX = X;
			iReport.wAxisY = Y;
			iReport.wAxisZ = Z;
			iReport.wAxisXRot = RX;
			if (!UpdateVJD(iInterface, (PVOID)&iReport))
			{
				_tprintf("Feeding vJoy device number %d failed - try to enable device then press enter\n", iInterface);
				getchar();
				AcquireVJD(iInterface);
			}
	}

void vJoyInput::AccelerationLogic() {
	if (UseMouse == 1) {
		if (rInput.IsLeftMouseButtonDown() && Y < 32767) {
			Y += AttackTimeThrottle;
		}
		if (!rInput.IsLeftMouseButtonDown() && Y > 1) {
			Y -= ReleaseTimeThrottle;
		}
		if (rInput.IsRightMouseButtonDown() && Z < 32767) {
			Z += AttackTimeBreak;
		}
		if (!rInput.IsRightMouseButtonDown() && Z > 1) {
			Z -= ReleaseTimeBreak;
		}
	}
	else {
		if (rInput.IsAlphabeticKeyDown(ThrottleKey - 0x41) && Y < 32767) {
			Y += AttackTimeThrottle;
		}
		if (!rInput.IsAlphabeticKeyDown(ThrottleKey - 0x41) && Y > 1) {
			Y -= ReleaseTimeThrottle;
		}
		if (rInput.IsAlphabeticKeyDown(BreakKey - 0x41) && Z < 32767) {
			Z += AttackTimeBreak;
		}
		if (!rInput.IsAlphabeticKeyDown(BreakKey - 0x41) && Z > 1) {
			Z -= ReleaseTimeBreak;
		}
	}
	if (rInput.IsAlphabeticKeyDown(ClutchKey - 0x41) && RX < 32767) {
		RX += AttackTimeClutch;
	}
	if (!rInput.IsAlphabeticKeyDown(ClutchKey - 0x41) && RX > 1) {
		RX -= ReleaseTimeClutch;
	}
	if (rInput.IsAlphabeticKeyDown(MouseLockKey - 0x41)) {
		SleepEx(250,!(rInput.IsAlphabeticKeyDown(MouseLockKey - 0x41)));
		if (CursorLocked == false) {
			CursorLocked = true;
		}
		else {
			CursorLocked = false;
		}
	}
	if (CursorLocked == true) {
		SetCursorPos(0, 0);
	}
	else {

	}
}

void ReadConfigFile() {


	std::string line_;
	std::ifstream file_("config.txt");
	if(file_.is_open()){
		while (getline(file_, line_)) {
			std::stringstream ss(line_);
			std::string tmp;
			int value;
			char c;
			ss >> tmp >> c >> value;

			if (tmp == "Sensitivity") {
				Sensitivity = value;
			}
			else if(tmp == "AttackTimeThrottle") {
				AttackTimeThrottle = value;
			}
			else if (tmp == "ReleaseTimeThrottle") {
				ReleaseTimeThrottle = value;
			}
			else if (tmp == "AttackTimeBreak") {
				AttackTimeBreak = value;
			}
			else if (tmp == "ReleaseTimeBreak") {
				ReleaseTimeBreak = value;
			}
			else if (tmp == "AttackTimeClutch") {
				AttackTimeClutch = value;
			}
			else if (tmp == "ReleaseTimeClutch") {
				ReleaseTimeClutch = value;
			}
			else if (tmp == "ThrottleKey") {
				ThrottleKey = value;
			}
			else if (tmp == "BreakKey") {
				BreakKey = value;
			}
			else if (tmp == "ClutchKey") {
				ClutchKey = value;
			}
			else if (tmp == "MouseLockKey") {
				MouseLockKey = value;
			}
			else if (tmp == "UseMouse") {
				UseMouse = value;
			}

		}
		std::cout << "==================================\n";
		std::cout << "Sensitivity = " << Sensitivity << "\n" << "Throttle Attack Time = " << AttackTimeThrottle << "\n" << "Throttle Release Time = " << ReleaseTimeThrottle << "\n" << "Break Attack Time = " << AttackTimeBreak << "\n" << "Break Release Time = " << ReleaseTimeBreak << "\n" << "Clutch Attack Time = " << AttackTimeClutch << "\n" << "Clutch Release Time = " << ReleaseTimeClutch << "\n" << "Throttle key = " << ThrottleKey << "\n" << "Break key = " << BreakKey << "\n" << "Clutch key = " << ClutchKey << "\n" << "Mouse Lock key = " << MouseLockKey << "\n" << "Use Mouse = " << UseMouse << "\n";
		std::cout << "==================================\n";
		file_.close();
	}
	else {
		std::cout << "Config file not found\n";
	}
	

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:

		RAWINPUTDEVICE m_Rid[2];

		//Keyboard
		m_Rid[0].usUsagePage = 1;
		m_Rid[0].usUsage = 6;
		m_Rid[0].dwFlags = RIDEV_INPUTSINK;
		m_Rid[0].hwndTarget = hwnd;

		// Mouse
		m_Rid[1].usUsagePage = 1;
		m_Rid[1].usUsage = 2;
		m_Rid[1].dwFlags = RIDEV_INPUTSINK;
		m_Rid[1].hwndTarget = hwnd;

		RegisterRawInputDevices(m_Rid, 2, sizeof(RAWINPUTDEVICE));

		break;
	case WM_INPUT:
		rInput.GetData(lParam);
		//Mouse
		if ((X += (rInput.GetMouseChangeX()*Sensitivity)) < 32767 && (X += (rInput.GetMouseChangeX()*Sensitivity)) > 0) 
		{
			X += (rInput.GetMouseChangeX()*Sensitivity);
		}
		else
		{
			if ((X += (rInput.GetMouseChangeX()*Sensitivity)) > 32767)
			{
				X = 32766;
			}
			if ((X += (rInput.GetMouseChangeX()*Sensitivity)) < 1)
			{
				X = 1;
			}
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
	case WM_DESTROY:
		DestroyWindow(hwnd);
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	static const char* class_name = "DUMMY_CLASS";
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;        // function which will handle messages
	wc.hInstance = hInstance;
	wc.lpszClassName = class_name;
	if (RegisterClassEx(&wc)) {
		CreateWindowEx(0, class_name, "dummy_name", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	}
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::ios::sync_with_stdio();
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	vJ.TestDriver();
	vJ.TestVirtualDevices();
	vJ.AccuireDevice();
	ReadConfigFile();


	while (true) {
		// Step 3: The Message Loop
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		
		//vjoylogic here
		Sleep(2);

		vJ.AccelerationLogic();
		vJ.FeedDevice();

		if (Msg.message == WM_QUIT || Msg.message == WM_DESTROY)
		{
			break;
		}

		}
		
	}
