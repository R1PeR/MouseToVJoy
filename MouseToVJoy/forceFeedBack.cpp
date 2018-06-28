#include <windows.h>
#include <string>
#include "forcefeedback.h"

using namespace std;
// Convert Packet type to String
BOOL ForceFeedBack::packetType2Str(FFBPType type, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (type)
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
		Str = "PID Device Control";
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
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Convert Effect type to String
BOOL ForceFeedBack::effectType2Str(FFBEType type, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (type)
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
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Convert PID Device Control to String
BOOL ForceFeedBack::devCtrl2Str(FFB_CTRL ctrl, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (ctrl)
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
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Convert Effect operation to string
BOOL ForceFeedBack::effectOpStr(FFBOP op, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = "";

	switch (op)
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
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Polar values (0x00-0xFF) to Degrees (0-360)
int ForceFeedBack::polar2Deg(BYTE polar)
{
	return ((UINT)polar * 360) / 255;
}

// Convert range 0x00-0xFF to 0%-100%
int ForceFeedBack::byte2Percent(BYTE inByte)
{
	return ((UINT)inByte * 100) / 255;
}

// Convert One-Byte 2's complement input to integer
int ForceFeedBack::twosCompByte2Int(BYTE in)
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

// Convert One-Byte 2's complement input to integer
int ForceFeedBack::twosCompWord2Int(WORD in)
{
	int tmp;
	WORD inv = ~in;
	BOOL isNeg = in >> 15;
	if (isNeg)
	{
		tmp = (int)(inv);
		tmp = -1 * tmp;
		return tmp - 1;
	}
	else
		return (int)in;
}
// Convert Ffb Calls into FFBSIZE struct
void CALLBACK ForceFeedBack::ffbToVJoy(PVOID data, PVOID userData)
{
	/////// Packet Device ID, and Type Block Index (if exists)
#pragma region Packet Device ID, and Type Block Index
	int DeviceID, BlockIndex;
	FFBPType	Type;
	TCHAR	TypeStr[100];
#pragma endregion


	/////// Effect Report
#pragma region Effect Report
#pragma warning( push )
#pragma warning( disable : 4996 )
	FFB_EFF_CONST Effect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Const((FFB_DATA *)data, &Effect))
	{
		
	};
#pragma endregion
#pragma region PID Device Control
	FFB_CTRL	Control;
	TCHAR	CtrlStr[100];
	if (ERROR_SUCCESS == Ffb_h_DevCtrl((FFB_DATA *)data, &Control) && devCtrl2Str(Control, CtrlStr))
	{

	}
#pragma endregion
#pragma region Effect Operation
	FFB_EFF_OP	Operation;
	TCHAR	EffOpStr[100];
	if (ERROR_SUCCESS == Ffb_h_EffOp((FFB_DATA *)data, &Operation) && effectOpStr(Operation.EffectOp, EffOpStr))
	{

	};
#pragma endregion
#pragma region Global Device Gain
	BYTE Gain;
	if (ERROR_SUCCESS == Ffb_h_DevGain((FFB_DATA *)data, &Gain)) {

	}

#pragma endregion
#pragma region Condition
	FFB_EFF_COND Condition;
	if (ERROR_SUCCESS == Ffb_h_Eff_Cond((FFB_DATA *)data, &Condition))
	{

	}
#pragma endregion
#pragma region Envelope
	FFB_EFF_ENVLP Envelope;
	if (ERROR_SUCCESS == Ffb_h_Eff_Envlp((FFB_DATA *)data, &Envelope))
	{

	};

#pragma endregion
#pragma region Periodic
	FFB_EFF_PERIOD EffPrd;
	if (ERROR_SUCCESS == Ffb_h_Eff_Period((FFB_DATA *)data, &EffPrd))
	{
		_ffbSize.setEffectType("Period");
		_ffbSize.setMagnitude(EffPrd.Magnitude);
		_ffbSize.setDirection(Effect.DirX);
		_ffbSize.setOffset(twosCompWord2Int(static_cast<WORD>(EffPrd.Offset)));
		_ffbSize.setPeriod(EffPrd.Period);
		_ffbSize.setDuration(Effect.Duration);
	};
#pragma endregion

#pragma region Effect Type
	FFBEType EffectType;
	if (ERROR_SUCCESS == Ffb_h_EffNew((FFB_DATA *)data, &EffectType))
	{

	};

#pragma endregion

#pragma region Ramp Effect
	FFB_EFF_RAMP RampEffect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Ramp((FFB_DATA *)data, &RampEffect))
	{

	};

#pragma endregion

#pragma region Constant Effect
	FFB_EFF_CONSTANT ConstantEffect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Constant((FFB_DATA *)data, &ConstantEffect))
	{

		_ffbSize.setEffectType("Constant");
		_ffbSize.setMagnitude((INT16)ConstantEffect.Magnitude);
		_ffbSize.setDirection(polar2Deg(Effect.Direction));
		_ffbSize.setOffset(0);
		_ffbSize.setPeriod(0);
		_ffbSize.setDuration(Effect.Duration);
	};
#pragma endregion
}