#include <windows.h>
#include <iostream>
#include <stdio.h>
#include "fileRead.h"
#include "vjoy.h"
#include "mousetovjoy.h"
#include "input.h"


const char g_szClassName[] = "myWindowClass";
HWND hwnd;
WNDCLASSEX wc;
MSG Msg;
vJoy vJ;
mouseToVjoy mTV;
cInputDevices rInput;
fileRead fR;
INT X, Y, Z, RX;
DOUBLE C;
using namespace std;
void InitializationCode() {
	
	vJ.testDriver();
	vJ.testVirtualDevices(1);
	vJ.accuireDevice(1);
	string fileName = "config.txt";
	string checkArray[18] = {"Sensitivity", "AttackTimeThrottle", "ReleaseTimeThrottle", "AttackTimeBreak", "ReleaseTimeBreak", "AttackTimeClutch", "ReleaseTimeClutch", "ThrottleKey", "BreakKey", "ClutchKey", "MouseLockKey", "MouseCenterKey", "UseMouse", "AccelerationThrottle", "AccelerationBreak", "AccelerationClutch", "CenterMultiplier", "UseCenterReduction"};
	fR.newFile(fileName, checkArray);
	printf("==================================\n");
	printf("Sensitivity = %.2f \n",fR.result(0));
	printf("Throttle Attack Time = %.0f \n", fR.result(1));
	printf("Throttle Release Time = %.0f \n", fR.result(2));
	printf("Break Attack Time = %.0f \n", fR.result(3));
	printf("Break Release Time = %.0f \n", fR.result(4));
	printf("Clutch Attack Time = %.0f \n", fR.result(5));
	printf("Clutch Release Time = %.0f \n", fR.result(6));
	printf("Throttle key = %.0f \n", fR.result(7));
	printf("Break key = %.0f \n", fR.result(8));
	printf("Clutch key = %.0f \n", fR.result(9));
	printf("Mouse Lock key = %.0f \n", fR.result(10));
	printf("Mouse Center key = %.0f \n", fR.result(11));
	printf("Use Mouse = %.0f \n", fR.result(12));
	printf("Use Center Reduction = %0.0f \n", fR.result(17));
	printf("Acceleration Throttle = %.2f \n", fR.result(13));
	printf("Acceleration Break = %.2f \n", fR.result(14));
	printf("Acceleration Clutch = %.2f \n", fR.result(15));
	printf("Center Multiplier = %.2f \n", fR.result(16));
	cout << "==================================\n";
	C = fR.result(16);
}
void UpdateCode() {
	mTV.inputLogic(rInput, X, Y, Z, RX, fR.result(1), fR.result(2), fR.result(3), fR.result(4), fR.result(5), fR.result(6), fR.result(7), fR.result(8), fR.result(9), fR.result(10), fR.result(11), fR.result(12), fR.result(13), fR.result(14), fR.result(15));
	vJ.feedDevice(1, X, Y, Z, RX);
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
			mTV.mouseLogic(rInput, X, fR.result(0), C, fR.result(17));
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
	ios::sync_with_stdio();
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	InitializationCode();
	


	while (true) {
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}

		Sleep(2);
		UpdateCode();

		if (Msg.message == WM_QUIT || Msg.message == WM_DESTROY)
		{
			break;
		}

		}
		
	}

