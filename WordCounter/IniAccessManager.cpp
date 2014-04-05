#include "stdafx.h"
#include "IniAccessManager.h"


IniAccessManager::IniAccessManager()
{
}


IniAccessManager::~IniAccessManager()
{
}

void IniAccessManager::load(string filename) {
	ifstream infile(filename);
	string line, key, value;
	while (getline(infile, line)) {
		// []: ignore

		// A=OOOXXXX
		int seperateIndex = line.find_first_of("=");
		key = line.substr(0, seperateIndex);
		value = line.substr(seperateIndex + 1);
		cout << "key: " << key << ", value: " << value << endl;
		iniMap[key] = value;
	}
	infile.close();
}

int IniAccessManager::getIntWithKey(string keyName) const {
	if (iniMap.find(keyName) != iniMap.end()) {		
		//iniMap[keyName];
	}
	return 0;
}

set<string> IniAccessManager::getStringSetWithKey(string keyName, string seperator) const {
	set<string> retSet;
	return retSet;
}

bool IniAccessManager::getBoolWithKey(string keyName) const {
	return false;
}