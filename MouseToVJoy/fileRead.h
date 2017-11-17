#pragma once
#ifndef FILEREAD_H
#define FILEREAD_H
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <stack>

using namespace std;
/*Funtion takes two strings and returns array of numbers from that file.
  "fileName" is name of the file that will be opened,
  "checkArray" is array of strings that will be read(max 32 strings),
  function works like this: opens fileName then goes by every line in the file in this way:
  "/string name/ = /value/" eg. "Sensitivity = 2",
  then it checks if /string name/ is equal to any string from checkArray, then parses it into resultArray.
  Result order is the same as checkArray order.
  Eg. checkArray = "Sensitivity, Multiplier, Divider" then no matter the order in file,
  output will be restultArray[Sensitivity, Multiplier, Divider etc.].
  To acces result just use funtion result[number] eg. result[0] for Sensitivity.
*/
class fileRead{
public:
	void newFile(string fileName, string checkArray[]);

	double result(int number) { return resultArray[number]; };

private:
	double resultArray[32];
};
#endif