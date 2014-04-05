#pragma once
#include <string>
#include <set>

class FileAccessManager;
class IniAccessManager;

using namespace std;

class WordCounter
{
public:
	WordCounter(/*const ManagerFactory& factory*/);
	~WordCounter();
	void load(string filename);
	void loadIni(string filename);
	int query(string str) const;

	void setFileAccessManager(FileAccessManager *fileMgr);
	void setIniAccessManager(IniAccessManager *iniMgr);

private:
	map<string, int> wordCountMap;
	int minWordLength;
	set<string> excludedWordList;
	bool hasMinWordLength;
	bool isCaseSensitive;	
	FileAccessManager *fileAccessMgr = 0;
	IniAccessManager *iniAccessMgr = 0;
};

class ManagerFactory {
public:
	ManagerFactory() {}
	virtual ~ManagerFactory() {}
	virtual FileAccessManager *getFileAccessManager() const;
	virtual IniAccessManager *getIniAccessManager() const;
};
