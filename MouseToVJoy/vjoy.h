#pragma once
#ifndef VJOY_H
#define VJOY_H
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <windows.h>
#include <basetyps.h>
#include "public.h"
#include "vjoyinterface.h"
/* Class that does everything with the vjoy, testing, accuiring, feeding.*/


class vJoy {
public:
	//Tests if the driver version is equal to dll version.
	int testDriver();
	//Tests if UINT iInterface is existing.
	int	testVirtualDevices(UINT iInterface);
	//If UINT iInterface is existing, tries to accuire it.
	int accuireDevice(UINT iInterface);
	//If UINT iInterface exist, enable FFB to device.
	int enableFFB(UINT iInterface);
	//When UINT iInterface is accuired, feeds vars X Y Z RX to Axises X Y Z RX.
	void feedDevice(UINT iInterface, INT X, INT Y, INT Z, INT RX, BOOL BUTTON1, BOOL BUTTON2, BOOL BUTTON3, INT *FFBSIZE);

private:
	//Gets status from vjoy device using vjoyinterface.dll.
	VjdStat status;
	//Creates new joystick position array named iReport, used to feed all the data at the same time.
	JOYSTICK_POSITION_V2 iReport;
};
#endif