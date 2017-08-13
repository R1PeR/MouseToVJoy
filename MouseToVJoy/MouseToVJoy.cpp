#include "mousetovjoy.h"
#include <iostream>
#include <math.h>
double CenterMultiplier;
DOUBLE SteeringMax = 16384, SteeringMin = -16384;


void mouseToVjoy::inputLogic(cInputDevices Input, INT &X, INT &Y, INT &Z, INT &RX, DOUBLE AttackTimeThrottle, DOUBLE ReleaseTimeThrottle, DOUBLE AttackTimeBreak, DOUBLE ReleaseTimeBreak, DOUBLE AttackTimeClutch, DOUBLE ReleaseTimeClutch, DOUBLE ThrottleKey, DOUBLE BreakKey, DOUBLE ClutchKey, DOUBLE MouseLockKey, DOUBLE MouseCenterKey, DOUBLE UseMouse, DOUBLE AccelerationThrottle, DOUBLE AccelerationBreak, DOUBLE AccelerationClutch) {

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
		if (Input.IsAlphabeticKeyDown(ThrottleKey - 0x41) && Y < 32767) {
			Y = (Y + AttackTimeThrottle) * AccelerationThrottle;
		}
		if (!Input.IsAlphabeticKeyDown(ThrottleKey - 0x41) && Y > 1) {
			Y = (Y - AttackTimeThrottle) / AccelerationThrottle;
		}
		if (Input.IsAlphabeticKeyDown(BreakKey - 0x41) && Z < 32767) {
			Z = (Z + AttackTimeThrottle) * AccelerationBreak;
		}
		if (!Input.IsAlphabeticKeyDown(BreakKey - 0x41) && Z > 1) {
			Z = (Z - AttackTimeThrottle) / AccelerationBreak;
		}
		}
		if (Input.IsAlphabeticKeyDown(ClutchKey - 0x41) && RX < 32767) {
			RX = (RX + AttackTimeClutch) * AccelerationClutch;
		}
		if (!Input.IsAlphabeticKeyDown(ClutchKey - 0x41) && RX > 1) {
			RX = (RX - ReleaseTimeClutch) / AccelerationClutch;
		}
		if (Input.IsAlphabeticKeyDown(MouseLockKey - 0x41)) {
			SleepEx(250, !(Input.IsAlphabeticKeyDown(MouseLockKey - 0x41)));
		if (CursorLocked == false) {
		CursorLocked = true;
	}
	else {
	CursorLocked = false;
	}
	}
	if (Input.IsAlphabeticKeyDown(MouseCenterKey - 0x41)) {
	SleepEx(250, !(Input.IsAlphabeticKeyDown(MouseCenterKey - 0x41)));
	X = (32766 / 2);
	}
	if (CursorLocked == true) {
	SetCursorPos(0, 0);
	}
}

void mouseToVjoy::mouseLogic(cInputDevices Input, INT &X, DOUBLE Sensitivity, DOUBLE SensitivityCenterReduction, INT UseCenterReduction) {
	
	X = X - 16384;
	if (X > 0) {
		CenterMultiplier = pow(SensitivityCenterReduction, (1 - (X / SteeringMax))) ;
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