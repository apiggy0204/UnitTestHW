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
	void load(const string& filename);
	void loadIni(const string& filename);
	int query(const string& str) const;

	void setFileAccessManager(FileAccessManager *fileMgr);
	void setIniAccessManager(IniAccessManager *iniMgr);

private:
	map<string, int> wordCountMap;
	unsigned int minWordLength;
	set<string> excludedWordList;
	bool hasMinWordLength;
	bool isCaseSensitive;	
	FileAccessManager *fileAccessMgr = 0;
	IniAccessManager *iniAccessMgr = 0;

	bool isExcludedWord(const string& str) const;
};

class ManagerFactory {
public:
	ManagerFactory() {}
	virtual ~ManagerFactory() {}
	virtual FileAccessManager *getFileAccessManager() const;
	virtual IniAccessManager *getIniAccessManager() const;
};
