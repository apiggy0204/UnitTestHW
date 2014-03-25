#pragma once
#include <string>

using namespace std;

class Log {
public:
	Log();
	~Log();
	virtual void write(string str);
};

