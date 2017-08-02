#pragma once
#ifndef VJOYINPUT_H
#define VJOYINPUT_H

#include "vjoyinterface.h"
class vJoyInput {
public:
	int TestDriver();
	int	TestVirtualDevices();
	int AccuireDevice();
	void FeedDevice();
	void InputLogic();
};
#endif