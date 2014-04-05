#pragma once
#include <string>
#include <set>

class FileAccessManager;
class IniAccessManager;

using namespace std;

class WordCounter
{
public:
	WordCounter();
	~WordCounter();
	void load(string filename);
	int query(string str) const;

private:
	map<string, int> wordCountMap;
	int minWordLength;
	set<string> excludedWordList;
	bool isCaseSensitive;
	FileAccessManager *fileAccessMgr;
	IniAccessManager *iniAccessMgr;
};

