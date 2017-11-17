#pragma once
#ifndef MOUSETOVJOY_H
#define MOUSETOVJOY_H
#include "input.h"

/* Basic funtion that gets data, then processes it and modifies inputs.*/

class mouseToVjoy {
public:
	//Function responsible for getting and modifying vars for throttle, break, clutch.
	void inputLogic(cInputDevices Input, INT &X, INT &Y, INT &Z, INT &RX, BOOL &BUTTON1, BOOL &BUTTON2, BOOL &BUTTON3, DOUBLE AttackTimeThrottle, DOUBLE ReleaseTimeThrottle, DOUBLE AttackTimeBreak, DOUBLE ReleaseTimeBreak, DOUBLE AttackTimeClutch, DOUBLE ReleaseTimeClutch, DOUBLE ThrottleKey, DOUBLE BreakKey, DOUBLE ClutchKey, DOUBLE GearShiftUpKey, DOUBLE GearShiftDownKey, DOUBLE HandBrakeKey, DOUBLE MouseLockKey, DOUBLE MouseCenterKey, DOUBLE UseMouse, DOUBLE AccelerationThrottle, DOUBLE AccelerationBreak, DOUBLE AccelerationClutch);
	//Function responsible for getting and modifying vars for steering wheel.
	void mouseLogic(cInputDevices Input, INT &X, DOUBLE Sensitivity, DOUBLE CenterMultiplier, INT UseCenterReduction);
	//Gets if the Cursor is locked then, sets cursor in cords 0,0 every input.
	bool CursorLocked;
	//Gets if the Center Reduction is used, when used function mouselogic() uses linear algorithm to calculate sensitivity, the more the joystick is centered the sloweer the sensitivity(max 1.0 of the sensitivity.), the further the faster the sensitivity(max = CenterReduction value * Sensitivity )
	bool UseCenterReduction;
};
#endif