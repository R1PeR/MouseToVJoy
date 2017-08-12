#include "fileRead.h"
void fileRead::newFile(string fileName, string checkArray[]) {
	string line_;
	ifstream file_(fileName);
	if (file_.is_open()) {
		while (getline(file_, line_)) {
			stringstream ss(line_);
			string tmp;
			double value;
			char c;
			ss >> tmp >> c >> value;
			for(int i = 0; i < 32; i++){
				if (tmp == checkArray[i]) {
					resultArray[i] = value;
				}
			}
		}
		file_.close();
	}
	else{
		printf("Config file not found\n");
	}
}