#include <windows.h>
#include <iostream>
#include <stdio.h>
#include "fileRead.h"
#include "vjoy.h"
#include "mousetovjoy.h"
#include "public.h"
#include "vjoyinterface.h"
#include "input.h"
//#include "forceFeedBack.h"

const char g_szClassName[] = "myWindowClass";
HWND hwnd;
WNDCLASSEX wc;
MSG Msg;
vJoy vJ;
mouseToVjoy mTV;
cInputDevices rInput;
fileRead fR;
//forceFeedBack fFB;
INT X, Y, Z, RX;
BOOL BUTTON1, BUTTON2, BUTTON3;
string keyCodeName;
INT ffbSize = 0;

using namespace std;
void InitializationCode() {
	//Code that is run only once, tests vjoy device, reads config file and prints basic out accuired vars.
	
	BOOL PacketType2Str(FFBPType Type, LPTSTR OutStr);

	BOOL EffectType2Str(FFBEType Type, LPTSTR OutStr);

	BOOL DevCtrl2Str(FFB_CTRL Ctrl, LPTSTR OutStr);

	BOOL EffectOpStr(FFBOP Op, LPTSTR OutStr);

	int Polar2Deg(BYTE Polar);

	int Byte2Percent(BYTE InByte);

	int TwosCompByte2Int(BYTE in);


	void CALLBACK FfbFunction(PVOID data);

	void CALLBACK FfbFunction1(PVOID data, PVOID userdata);

	void CALLBACK FfbToVjoy(PVOID data, PVOID userdata);

	UINT DevID = 1;
	
	vJ.testDriver();
	vJ.testVirtualDevices(DevID);
	vJ.accuireDevice(DevID);
	vJ.enableFFB(DevID);
	FfbRegisterGenCB(FfbToVjoy, &DevID);

	string fileName = "config.txt";
	string checkArray[21] = {"Sensitivity", "AttackTimeThrottle", "ReleaseTimeThrottle", "AttackTimeBreak", "ReleaseTimeBreak", "AttackTimeClutch", "ReleaseTimeClutch", "ThrottleKey", "BreakKey", "ClutchKey", "GearShiftUpKey", "GearShiftDownKey", "HandBrakeKey", "MouseLockKey", "MouseCenterKey", "UseMouse","UseCenterReduction" , "AccelerationThrottle", "AccelerationBreak", "AccelerationClutch", "CenterMultiplier"};
	fR.newFile(fileName, checkArray);
	printf("==================================\n");
	printf("Sensitivity = %.2f \n",fR.result(0));
	printf("Throttle Attack Time = %.0f \n", fR.result(1));
	printf("Throttle Release Time = %.0f \n", fR.result(2));
	printf("Break Attack Time = %.0f \n", fR.result(3));
	printf("Break Release Time = %.0f \n", fR.result(4));
	printf("Clutch Attack Time = %.0f \n", fR.result(5));
	printf("Clutch Release Time = %.0f \n", fR.result(6));
	printf("Throttle key = %d \n", (int)fR.result(7));
	printf("Break key = %d \n", (int)fR.result(8));
	printf("Clutch key = %d \n", (int)fR.result(9));
	printf("Gear Shift Up key = %d \n", (int)fR.result(10));
	printf("Gear Shift Down key = %d \n", (int)fR.result(11));
	printf("Hand Brake Key = %d \n", (int)fR.result(12));
	printf("Mouse Lock key = %d \n", (int)fR.result(13));
	printf("Mouse Center key = %d \n", (int)fR.result(14));
	printf("Use Mouse = %d \n", (int)fR.result(15));
	printf("Use Center Reduction = %d \n", (int)fR.result(16));
	printf("Acceleration Throttle = %.2f \n", fR.result(17));
	printf("Acceleration Break = %.2f \n", fR.result(18));
	printf("Acceleration Clutch = %.2f \n", fR.result(19));
	printf("Center Multiplier = %.2f \n", fR.result(20));
	printf("==================================\n");
}

void UpdateCode() {
	//Code that is run every time program gets an message from enviroment(mouse movement, mouse click etc.), manages input logic and feeding device.
	mTV.inputLogic(rInput, X, Y, Z, RX, BUTTON1, BUTTON2, BUTTON3, fR.result(1), fR.result(2), fR.result(3), fR.result(4), fR.result(5), fR.result(6), fR.result(7), fR.result(8), fR.result(9), fR.result(10), fR.result(11), fR.result(12), fR.result(13), fR.result(14),fR.result(16), fR.result(17), fR.result(18), fR.result(19));
	X = X + ffbSize;
	vJ.feedDevice(1, X, Y, Z, RX, BUTTON1, BUTTON2, BUTTON3, &ffbSize);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
		//Creating new twi raw input devices
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
		//Then window recives input message get data for rinput device and run mouse logic function.
			rInput.GetData(lParam);
			mTV.mouseLogic(rInput, X, fR.result(0), fR.result(20), fR.result(16));
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
	
	//invisible window initialization to be able to recive raw input even if the window is not focused.
	static const char* class_name = "DUMMY_CLASS";
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;        // function which will handle messages
	wc.hInstance = hInstance;
	wc.lpszClassName = class_name;
	if (RegisterClassEx(&wc)) {
		CreateWindowEx(0, class_name, "dummy_name", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	}
	//Allocating console to process and redirect every stdout, stdin to it.
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	ios::sync_with_stdio();
	//Show invisible window, update it, then do initialization code.
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	InitializationCode();
	

	//Loop on PeekMessage instead of GetMessage to avoid overflow.
	while (true) {
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		//To optimalize cpu usade wait 2 milisecond before running update code.
		Sleep(2);
		UpdateCode();
		//	if (BUTTON1) printf("btn1\n"); DEBUG
		//	if (BUTTON2) printf("btn2\n"); DEBUG
		//If Message is equal to quit or destroy, break loop and end program.
		if (Msg.message == WM_QUIT || Msg.message == WM_DESTROY)
		{
			break;
		}

	}
		
}
// Convert Packet type to String
BOOL PacketType2Str(FFBPType Type, LPTSTR OutStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (Type)
	{
	case PT_EFFREP:
		Str = "Effect Report";
		break;
	case PT_ENVREP:
		Str = "Envelope Report";
		break;
	case PT_CONDREP:
		Str = "Condition Report";
		break;
	case PT_PRIDREP:
		Str = "Periodic Report";
		break;
	case PT_CONSTREP:
		Str = "Constant Force Report";
		break;
	case PT_RAMPREP:
		Str = "Ramp Force Report";
		break;
	case PT_CSTMREP:
		Str = "Custom Force Data Report";
		break;
	case PT_SMPLREP:
		Str = "Download Force Sample";
		break;
	case PT_EFOPREP:
		Str = "Effect Operation Report";
		break;
	case PT_BLKFRREP:
		Str = "PID Block Free Report";
		break;
	case PT_CTRLREP:
		Str = "PID Device Contro";
		break;
	case PT_GAINREP:
		Str = "Device Gain Report";
		break;
	case PT_SETCREP:
		Str = "Set Custom Force Report";
		break;
	case PT_NEWEFREP:
		Str = "Create New Effect Report";
		break;
	case PT_BLKLDREP:
		Str = "Block Load Report";
		break;
	case PT_POOLREP:
		Str = "PID Pool Report";
		break;
	default:
		stat = FALSE;
		break;
	}

	if (stat)
		_tcscpy_s(OutStr, 100, Str);

	return stat;
}

// Convert Effect type to String
BOOL EffectType2Str(FFBEType Type, LPTSTR OutStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (Type)
	{
	case ET_NONE:
		stat = FALSE;
		break;
	case ET_CONST:
		Str = "Constant Force";
		break;
	case ET_RAMP:
		Str = "Ramp";
		break;
	case ET_SQR:
		Str = "Square";
		break;
	case ET_SINE:
		Str = "Sine";
		break;
	case ET_TRNGL:
		Str = "Triangle";
		break;
	case ET_STUP:
		Str = "Sawtooth Up";
		break;
	case ET_STDN:
		Str = "Sawtooth Down";
		break;
	case ET_SPRNG:
		Str = "Spring";
		break;
	case ET_DMPR:
		Str = "Damper";
		break;
	case ET_INRT:
		Str = "Inertia";
		break;
	case ET_FRCTN:
		Str = "Friction";
		break;
	case ET_CSTM:
		Str = "Custom Force";
		break;
	default:
		stat = FALSE;
		break;
	};

	if (stat)
		_tcscpy_s(OutStr, 100, Str);

	return stat;
}

// Convert PID Device Control to String
BOOL DevCtrl2Str(FFB_CTRL Ctrl, LPTSTR OutStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (Ctrl)
	{
	case CTRL_ENACT:
		Str = "Enable Actuators";
		break;
	case CTRL_DISACT:
		Str = "Disable Actuators";
		break;
	case CTRL_STOPALL:
		Str = "Stop All Effects";
		break;
	case CTRL_DEVRST:
		Str = "Device Reset";
		break;
	case CTRL_DEVPAUSE:
		Str = "Device Pause";
		break;
	case CTRL_DEVCONT:
		Str = "Device Continue";
		break;
	default:
		stat = FALSE;
		break;
	}
	if (stat)
		_tcscpy_s(OutStr, 100, Str);

	return stat;
}

// Convert Effect operation to string
BOOL EffectOpStr(FFBOP Op, LPTSTR OutStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (Op)
	{
	case EFF_START:
		Str = "Effect Start";
		break;
	case EFF_SOLO:
		Str = "Effect Solo Start";
		break;
	case EFF_STOP:
		Str = "Effect Stop";
		break;
	default:
		stat = FALSE;
		break;
	}

	if (stat)
		_tcscpy_s(OutStr, 100, Str);

	return stat;
}

// Polar values (0x00-0xFF) to Degrees (0-360)
int Polar2Deg(BYTE Polar)
{
	return ((UINT)Polar * 360) / 255;
}

// Convert range 0x00-0xFF to 0%-100%
int Byte2Percent(BYTE InByte)
{
	return ((UINT)InByte * 100) / 255;
}

// Convert One-Byte 2's complement input to integer
int TwosCompByte2Int(BYTE in)
{
	int tmp;
	BYTE inv = ~in;
	BOOL isNeg = in >> 7;
	if (isNeg)
	{
		tmp = (int)(inv);
		tmp = -1 * tmp;
		return tmp;
	}
	else
		return (int)in;
}

void CALLBACK FfbFunction(PVOID data)
{
	FFB_DATA * FfbData = (FFB_DATA *)data;
	int size = FfbData->size;
	printf("\nFFB Size %d\n", size);

	printf("Cmd:%08.8X ", FfbData->cmd);
	printf("ID:%02.2X ", FfbData->data[0]);
	printf("Size:%02.2d ", static_cast<int>(FfbData->size - 8));
	printf(" - ");
	for (UINT i = 0; i < FfbData->size - 8; i++)
		printf(" %02.2X", (UINT)FfbData->data);
	printf("\n");
}

void CALLBACK FfbFunction1(PVOID data, PVOID userdata)
{
	// Packet Header
	printf("\n ============= FFB Packet size Size %d =============\n", static_cast<int>(((FFB_DATA *)data)->size));

	/////// Packet Device ID, and Type Block Index (if exists)
#pragma region Packet Device ID, and Type Block Index
	int DeviceID, BlockIndex;
	FFBPType	Type;
	TCHAR	TypeStr[100];

	if (ERROR_SUCCESS == Ffb_h_DeviceID((FFB_DATA *)data, &DeviceID))
		printf("\n > Device ID: %d", DeviceID);
	if (ERROR_SUCCESS == Ffb_h_Type((FFB_DATA *)data, &Type))
	{
		if (!PacketType2Str(Type, TypeStr))
			printf("\n > Packet Type: %d", Type);
		else
			printf("\n > Packet Type: %s", TypeStr);

	}
	if (ERROR_SUCCESS == Ffb_h_EBI((FFB_DATA *)data, &BlockIndex))
		printf("\n > Effect Block Index: %d", BlockIndex);
#pragma endregion


	/////// Effect Report
#pragma region Effect Report
	FFB_EFF_CONST Effect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Report((FFB_DATA *)data, &Effect))
	{
		if (!EffectType2Str(Effect.EffectType, TypeStr))
			printf("\n >> Effect Report: %02x", Effect.EffectType);
		else
			printf("\n >> Effect Report: %s", TypeStr);

		if (Effect.Polar)
		{
			printf("\n >> Direction: %d deg (%02x)", Polar2Deg(Effect.Direction), Effect.Direction);


		}
		else
		{
			printf("\n >> X Direction: %02x", Effect.DirX);
			printf("\n >> Y Direction: %02x", Effect.DirY);
		};

		if (Effect.Duration == 0xFFFF)
			printf("\n >> Duration: Infinit");
		else
			printf("\n >> Duration: %d MilliSec", static_cast<int>(Effect.Duration));

		if (Effect.TrigerRpt == 0xFFFF)
			printf("\n >> Trigger Repeat: Infinit");
		else
			printf("\n >> Trigger Repeat: %d", static_cast<int>(Effect.TrigerRpt));

		if (Effect.SamplePrd == 0xFFFF)
			printf("\n >> Sample Period: Infinit");
		else
			printf("\n >> Sample Period: %d", static_cast<int>(Effect.SamplePrd));


		printf("\n >> Gain: %d%%", Byte2Percent(Effect.Gain));

	};
#pragma endregion
#pragma region PID Device Control
	FFB_CTRL	Control;
	TCHAR	CtrlStr[100];
	if (ERROR_SUCCESS == Ffb_h_DevCtrl((FFB_DATA *)data, &Control) && DevCtrl2Str(Control, CtrlStr))
		printf("\n >> PID Device Control: %s", CtrlStr);

#pragma endregion
#pragma region Effect Operation
	FFB_EFF_OP	Operation;
	TCHAR	EffOpStr[100];
	if (ERROR_SUCCESS == Ffb_h_EffOp((FFB_DATA *)data, &Operation) && EffectOpStr(Operation.EffectOp, EffOpStr))
	{
		_tprintf("\n >> Effect Operation: %s", EffOpStr);
		if (Operation.LoopCount == 0xFF)
			printf("\n >> Loop until stopped");
		else
			printf("\n >> Loop %d times", static_cast<int>(Operation.LoopCount));

	};
#pragma endregion
#pragma region Global Device Gain
	BYTE Gain;
	if (ERROR_SUCCESS == Ffb_h_DevGain((FFB_DATA *)data, &Gain))
		printf("\n >> Global Device Gain: %d", Byte2Percent(Gain));

#pragma endregion
#pragma region Condition
	FFB_EFF_COND Condition;
	if (ERROR_SUCCESS == Ffb_h_Eff_Cond((FFB_DATA *)data, &Condition))
	{
		if (Condition.isY)
			_tprintf("\n >> Y Axis");
		else
			_tprintf("\n >> X Axis");
		_tprintf("\n >> Center Point Offset: %d", TwosCompByte2Int(Condition.CenterPointOffset) * 10000 / 127);
		_tprintf("\n >> Positive Coefficient: %d", TwosCompByte2Int(Condition.PosCoeff) * 10000 / 127);
		_tprintf("\n >> Negative Coefficient: %d", TwosCompByte2Int(Condition.NegCoeff) * 10000 / 127);
		_tprintf("\n >> Positive Saturation: %d", Condition.PosSatur * 10000 / 255);
		_tprintf("\n >> Negative Saturation: %d", Condition.NegSatur * 10000 / 255);
		_tprintf("\n >> Dead Band: %d", Condition.DeadBand * 10000 / 255);
	}
#pragma endregion
#pragma region Envelope
	FFB_EFF_ENVLP Envelope;
	if (ERROR_SUCCESS == Ffb_h_Eff_Envlp((FFB_DATA *)data, &Envelope))
	{
		_tprintf("\n >> Attack Level: %d", Envelope.AttackLevel * 10000 / 255);
		_tprintf("\n >> Fade Level: %d", Envelope.FadeLevel * 10000 / 255);
		_tprintf("\n >> Attack Time: %d", static_cast<int>(Envelope.AttackTime));
		_tprintf("\n >> Fade Time: %d", static_cast<int>(Envelope.FadeTime));
	};

#pragma endregion
#pragma region Periodic
	FFB_EFF_PERIOD EffPrd;
	if (ERROR_SUCCESS == Ffb_h_Eff_Period((FFB_DATA *)data, &EffPrd))
	{
		_tprintf("\n >> Magnitude: %d", EffPrd.Magnitude * 10000 / 255);
		_tprintf("\n >> Offset: %d", TwosCompByte2Int(EffPrd.Offset) * 10000 / 127);
		_tprintf("\n >> Phase: %d", EffPrd.Phase * 3600 / 255);
		_tprintf("\n >> Period: %d", static_cast<int>(EffPrd.Period));
	};
#pragma endregion

#pragma region Effect Type
	FFBEType EffectType;
	if (ERROR_SUCCESS == Ffb_h_EffNew((FFB_DATA *)data, &EffectType))
	{
		if (EffectType2Str(EffectType, TypeStr))
			_tprintf("\n >> Effect Type: %s", TypeStr);
		else
			_tprintf("\n >> Effect Type: Unknown");
	}

#pragma endregion

#pragma region Ramp Effect
	FFB_EFF_RAMP RampEffect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Ramp((FFB_DATA *)data, &RampEffect))
	{
		_tprintf("\n >> Ramp Start: %d", TwosCompByte2Int(RampEffect.Start) * 10000 / 127);
		_tprintf("\n >> Ramp End: %d", TwosCompByte2Int(RampEffect.End) * 10000 / 127);
	};

#pragma endregion

	printf("\n");
	FfbFunction(data);
	printf("\n ====================================================\n");

}

void CALLBACK FfbToVjoy(PVOID data, PVOID userdata) {
		// Packet Header
		//printf("\n ============= FFB Packet size Size %d =============\n", static_cast<int>(((FFB_DATA *)data)->size));

		/////// Packet Device ID, and Type Block Index (if exists)
#pragma region Packet Device ID, and Type Block Index
		int DeviceID, BlockIndex;
		FFBPType	Type;
		TCHAR	TypeStr[100];

#pragma region Effect Report
		FFB_EFF_CONST Effect;
		if (ERROR_SUCCESS == Ffb_h_Eff_Report((FFB_DATA *)data, &Effect))
		{
			printf("\n");
			//FfbFunction(data);
			//printf("\n ====================================================\n");

			if (Polar2Deg(Effect.Direction) > 100) {
				ffbSize = 0;
			}
			else {
				ffbSize = 0;
			}
			printf("ForceFeedBack is equal to: %ld and degrees to: %d", NULL, Polar2Deg(Effect.Direction));
		}
}
