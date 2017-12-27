#pragma once
#pragma once
#ifndef FORCEFEEDBACK_H
#define FORCEFEEDBACK_H
#include <iostream>
#include <windows.h>
#include <stack>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <basetyps.h>
#include <cfgmgr32.h>
#include <Setupapi.h>
#include <strsafe.h>
#include <Newdev.h>
#include <INITGUID.H>
#include "vjoyinterface.h"


using namespace std;
class forceFeedBack {
public:
	void initialize(UINT DevID);
};
BOOL PacketType2Str(FFBPType Type, LPTSTR OutStr);

BOOL EffectType2Str(FFBEType Type, LPTSTR OutStr);

BOOL DevCtrl2Str(FFB_CTRL Ctrl, LPTSTR OutStr);

BOOL EffectOpStr(FFBOP Op, LPTSTR OutStr);

int Polar2Deg(BYTE Polar);

int Byte2Percent(BYTE InByte);

int TwosCompByte2Int(BYTE in);

void CALLBACK FfbFunction(PVOID data);

void CALLBACK FfbFunction1(PVOID data, PVOID userdata);
#endif