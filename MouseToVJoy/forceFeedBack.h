#pragma once
#ifndef FORCEFEEDBACK_H
#define FORCEFEEDBACK_H
#include "vjoy.h"
#include "vjoyinterface.h"
#include "input.h"
#include "ffbsize.h"
#include <Windows.h>

/* Basic funtion that gets data, then processes it and modifies inputs.*/

class ForceFeedBack{
public:
	// Convert Packet type to String
	BOOL packetType2Str(FFBPType Type, LPTSTR OutStr);
	// Convert Effect type to String
	BOOL effectType2Str(FFBEType Type, LPTSTR OutStr);
	// Convert PID Device Control to String
	BOOL devCtrl2Str(FFB_CTRL Ctrl, LPTSTR OutStr);
	// Convert Effect operation to string
	BOOL effectOpStr(FFBOP Op, LPTSTR OutStr);
	// Polar values (0x00-0xFF) to Degrees (0-360)
	int polar2Deg(BYTE Polar);
	// Convert range 0x00-0xFF to 0%-100%
	int byte2Percent(BYTE InByte);
	// Convert One-Byte 2's complement input to integer
	int twosCompByte2Int(BYTE in);
	// Convert One-Byte 2's complement input to integer
	int twosCompWord2Int(WORD in);
	FFBSIZE getFfbSize() { return _ffbSize; };
	void CALLBACK ffbToVJoy(PVOID data, PVOID userData);
private:
	LPCTSTR _ffbEffectName[13] = { "NONE", "Constant Force", "Ramp", "Square", "Sine", "Triangle", "Sawtooth Up",
		"Sawtooth Down", "Spring", "Damper", "Inertia", "Friction", "Custom Force" };
	FFBSIZE _ffbSize;
};
#endif