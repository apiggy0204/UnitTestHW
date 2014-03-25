#include "stdafx.h"
#include "Log.h"
#include <iostream>

Log::Log() {}

Log::~Log() {}

void Log::write(string str) {
	cout << str << endl;
}