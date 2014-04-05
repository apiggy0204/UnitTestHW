#pragma once
#include <string>

using namespace std;

class IniAccessManager
{
public:
	IniAccessManager();
	virtual ~IniAccessManager();
	
	virtual void load(string filename);
	virtual int getIntWithKey(string iniName) const;
	virtual set<string> getStringSetWithKey(string keyName, string delim = ";") const;
	virtual bool getBoolWithKey(string keyName) const;

private:
	map<string, string> iniMap;
};

