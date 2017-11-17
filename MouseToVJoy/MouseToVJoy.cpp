#include "mousetovjoy.h"
#include <iostream>
#include <math.h>
//define center multiplier to be able to modify it during function.
DOUBLE CenterMultiplier;
//define maximal value of vjoy axis.
DOUBLE SteeringMax = 16384, SteeringMin = -16384;


void mouseToVjoy::inputLogic(cInputDevices Input, INT &X, INT &Y, INT &Z, INT &RX, BOOL &BUTTON1, BOOL &BUTTON2, BOOL &BUTTON3, DOUBLE AttackTimeThrottle, DOUBLE ReleaseTimeThrottle, DOUBLE AttackTimeBreak, DOUBLE ReleaseTimeBreak, DOUBLE AttackTimeClutch, DOUBLE ReleaseTimeClutch, DOUBLE ThrottleKey, DOUBLE BreakKey, DOUBLE ClutchKey, DOUBLE GearShiftUpKey, DOUBLE GearShiftDownKey, DOUBLE HandBrakeKey, DOUBLE MouseLockKey, DOUBLE MouseCenterKey, DOUBLE UseMouse, DOUBLE AccelerationThrottle, DOUBLE AccelerationBreak, DOUBLE AccelerationClutch) {
	if (UseMouse == 1) {
		if (Input.IsLeftMouseButtonDown() && Y < 32767) {
			Y = (Y + AttackTimeThrottle ) * AccelerationThrottle;
		}
		if (!Input.IsLeftMouseButtonDown() && Y > 1) {
			Y = (Y - ReleaseTimeThrottle) / AccelerationThrottle;;
		}
		if (Input.IsRightMouseButtonDown() && Z < 32767) {
			Z = (Z + AttackTimeBreak) * AccelerationBreak;
		}
		if (!Input.IsRightMouseButtonDown() && Z > 1) {
			Z = (Z - ReleaseTimeBreak) / AccelerationBreak;
		}
	}
	else {
		if (Input.IsAlphabeticKeyDown(ThrottleKey) && Y < 32767) {
			Y = (Y + AttackTimeThrottle) * AccelerationThrottle;
		}
		if (!Input.IsAlphabeticKeyDown(ThrottleKey) && Y > 1) {
			Y = (Y - AttackTimeThrottle) / AccelerationThrottle;
		}
		if (Input.IsAlphabeticKeyDown(BreakKey) && Z < 32767) {
			Z = (Z + AttackTimeThrottle) * AccelerationBreak;
		}
		if (!Input.IsAlphabeticKeyDown(BreakKey) && Z > 1) {
			Z = (Z - AttackTimeThrottle) / AccelerationBreak;
		}
	}
	if (Input.IsAlphabeticKeyDown(ClutchKey) && RX < 32767) {
		RX = (RX + AttackTimeClutch) * AccelerationClutch;
	}
	if (!Input.IsAlphabeticKeyDown(ClutchKey) && RX > 1) {
		RX = (RX - ReleaseTimeClutch) / AccelerationClutch;
	}
	if (Input.IsAlphabeticKeyDown(MouseLockKey)) {
		SleepEx(250, !(Input.IsAlphabeticKeyDown(MouseLockKey)));
		if (CursorLocked == false) {
			CursorLocked = true;
		}
		else {
			CursorLocked = false;
		}
	}
	if (Input.IsAlphabeticKeyDown(MouseCenterKey)) {
		SleepEx(250, !(Input.IsAlphabeticKeyDown(MouseCenterKey)));
		X = (32766 / 2);
	}
	if (Input.IsAlphabeticKeyDown(GearShiftUpKey)) {
		BUTTON1 = true;
	}
	else BUTTON1 = false;
	if (Input.IsAlphabeticKeyDown(GearShiftDownKey)) {
		BUTTON2 = true;
	}
	else BUTTON2 = false;
	if (Input.IsAlphabeticKeyDown(HandBrakeKey)){
		BUTTON3 = true;
	}
	else BUTTON3 = false;
	if (CursorLocked == true) {
		SetCursorPos(0, 0);
	}
}

void mouseToVjoy::mouseLogic(cInputDevices Input, INT &X, DOUBLE Sensitivity, DOUBLE SensitivityCenterReduction, INT UseCenterReduction) {
	//vjoy max value is 0-32767 to make it easier to scale linear reduction/acceleration I subtract half of it so 16384 to make it -16384 to 16384.
	X = X - 16384;
	if (X > 0) {
		CenterMultiplier = pow(SensitivityCenterReduction, (1 - (X / SteeringMax)));
	}
	else if(X < 0){
		CenterMultiplier = pow(SensitivityCenterReduction, (1 - (X / SteeringMin)));
	}
	if (UseCenterReduction == 1) {
		X = X + ((Input.GetMouseChangeX() * Sensitivity) / CenterMultiplier);
	}
	else {
		X = X + (Input.GetMouseChangeX() * Sensitivity);
	}
	if (X > 16384) {
		X = 16384;
	}
	else if (X < -16384) {
		X = -16384;
	}
	X = X + 16384;
};