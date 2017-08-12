#pragma once
#ifndef VJOY_H
#define VJOY_H
#include <stdio.h>
#include <Windows.h>
#include "public.h"
#include "vjoyinterface.h"

class vJoy {
public:
	int testDriver();

	int	testVirtualDevices(UINT iInterface);

	int accuireDevice(UINT iInterface);

	void feedDevice(UINT iInterface, INT X, INT Y, INT Z, INT RX);

private:
	VjdStat status;
	JOYSTICK_POSITION_V2 iReport;
	
};
#endif