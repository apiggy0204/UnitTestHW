#pragma once
#include <map>
#include <string>

using namespace std;

class FileAccessManager
{
public:
	FileAccessManager();
	~FileAccessManager();
	typedef map<string, int> Map;
	Map getWordCountMap(string filename);
private:
	string removePunctuation(string str);
};

