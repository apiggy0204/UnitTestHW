#include "stdafx.h"
#include "IniAccessManager.h"
#include <sstream>

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
		int seperateIndex = 0;
		seperateIndex = line.find_first_of("=");
		if (seperateIndex != string::npos) {
			key = line.substr(0, seperateIndex);
			value = line.substr(seperateIndex + 1);
			cout << "key: " << key << ", value: " << value << endl;
			iniMap[key] = value;
		}
	}
	infile.close();
}

int IniAccessManager::getIntWithKey(string keyName) const {
	map<string, string>::const_iterator it;
	if ((it = iniMap.find(keyName)) != iniMap.end()) {		
		stringstream ss;
		int num;
		ss << it->second;
		ss >> num;
		return num;
	}

	return 0;
}

set<string> IniAccessManager::getStringSetWithKey(string keyName, string delim) const {
	set<string> retSet;
	map<string, string>::const_iterator it;
	if ((it = iniMap.find(keyName)) != iniMap.end()) {
		stringstream toSplit(it->second);
		string token;
		while (getline(toSplit, token, *delim.c_str())) {
			retSet.insert(token);
		}
	}

	return retSet;
}

bool IniAccessManager::getBoolWithKey(string keyName) const {
	map<string, string>::const_iterator it;
	if ((it = iniMap.find(keyName)) != iniMap.end()) {				
		if (it->second.find("true") != string::npos) {
			return true;
		}
	}

	return false;
}