#include "fileRead.h"
void FileRead::newFile(string fileName, string checkArray[]) {
	string line;
	ifstream file(fileName);
	if (file.is_open()) {
		while (getline(file, line)) {
			stringstream ss(line);
			string tmp;
			double value;
			char c;
			ss >> tmp >> c >> value;
			for(int i = 0; i < 32; i++){
				if (tmp == checkArray[i]) {
					_resultArray[i] = value;
				}
			}
		}
		file.close();
	}
	else{
		printf("Config file not found\n");
	}
}