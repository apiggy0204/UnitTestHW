#pragma once
#include <map>
#include <string>

using namespace std;

class FileAccessManager
{
public:
	FileAccessManager();
	virtual ~FileAccessManager();
	typedef map<string, int> Map;
	virtual Map getWordCountMap(string filename);
private:
	string removePunctuation(string str);
};

