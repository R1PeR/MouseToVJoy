#pragma once
#ifndef FFBSIZE_H
#define FFBSIZE_H
#include <string>
using namespace std;
struct FFBSIZE {
private:
	string _effectTypeStr;
	int _magnitudeVar;
	int _directionVar;
	int _offsetVar;
	int _periodVar;
	int _durationVar;
public:
	string getEffectType() { return _effectTypeStr; }
	int getMagnitude() { return _magnitudeVar; }
	int getDirection() { return _directionVar; }
	int getOffset() { return _offsetVar; }
	int getPeriod() { return _periodVar; }
	int getDuration() { return _durationVar; }
	void setEffectType(string type) { if (type != "NULL") { _effectTypeStr = type; } }
	void setMagnitude(int size) { if (size != NULL) { _magnitudeVar = size; } }
	void setDirection(int size) { if (size != NULL) { _directionVar = size; } }
	void setOffset(int size) { if (size != NULL) { _offsetVar = size; } }
	void setPeriod(int size) { if (size != NULL) { _periodVar = size; } }
	void setDuration(int size) { if (size != NULL) { _durationVar = size; } }
};
#endif