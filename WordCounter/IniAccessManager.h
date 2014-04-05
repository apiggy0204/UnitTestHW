#pragma once
#include <string>

using namespace std;

class IniAccessManager
{
public:
	IniAccessManager();
	virtual ~IniAccessManager();
	
	void load(string filename);
	int getIntWithKey(string iniName) const;
	set<string> getStringSetWithKey(string keyName, string seperator = ";") const;
	bool getBoolWithKey(string keyName) const;

private:
	map<string, string> iniMap;
};

