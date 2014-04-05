#include "stdafx.h"
#include "FileAccessManager.h"
#include <algorithm>
#include <functional>
#include <cctype>

FileAccessManager::FileAccessManager()
{
}


FileAccessManager::~FileAccessManager()
{
}

map<string, int> FileAccessManager::getWordCountMap(string filename) {	
	Map retmap;
	Map::iterator it;
	ifstream infile(filename);
	string buf;

	while (infile >> buf) {		
		buf = removePunctuation(buf);
		if ((it = retmap.find(buf)) != retmap.end()) {
			retmap[buf] += 1;
		}
		else {
			retmap[buf] = 1;
		}
	}
	infile.close();
	return retmap;
}

string FileAccessManager::removePunctuation(string str) {
	string result;
	std::remove_copy_if(str.begin(), str.end(),
		std::back_inserter(result), //Store output           
		std::ptr_fun<int, int>(&std::ispunct)
	);
	return result;
}