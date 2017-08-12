#pragma once
#ifndef MOUSETOVJOY_H
#define MOUSETOVJOY_H
#include "input.h"
class mouseToVjoy {
public:
	void inputLogic(cInputDevices Input, INT &X, INT &Y, INT &Z, INT &RX, DOUBLE AttackTimeThrottle, DOUBLE ReleaseTimeThrottle, DOUBLE AttackTimeBreak, DOUBLE ReleaseTimeBreak, DOUBLE AttackTimeClutch, DOUBLE ReleaseTimeClutch, DOUBLE ThrottleKey, DOUBLE BreakKey, DOUBLE ClutchKey, DOUBLE MouseLockKey, DOUBLE MouseCenterKey, DOUBLE UseMouse, DOUBLE AccelerationThrottle, DOUBLE AccelerationBreak, DOUBLE AccelerationClutch);
	void mouseLogic(cInputDevices Input, INT &X, DOUBLE Sensitivity, DOUBLE &CenterMultiplier, INT UseCenterReduction);
	bool CursorLocked;
	bool UseCenterReduction;
};
#endif