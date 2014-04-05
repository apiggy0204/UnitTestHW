// WordCounter.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "WordCounter.h"
#include "FileAccessManager.h"
#include "IniAccessManager.h"
#include "gtest/gtest.h"

TEST(FileAccessManagerTest, ReadSimpleFile) {
	FileAccessManager fileMgr;
	map<string, int> wordCountMap = fileMgr.getWordCountMap("hello_world.txt");
	EXPECT_EQ(wordCountMap["hello"], 1);
	EXPECT_EQ(wordCountMap["world"], 1);
}

TEST(FileAccessManagerTest, ReadFileWithPunctuation) {
	FileAccessManager fileMgr;
	map<string, int> wordCountMap = fileMgr.getWordCountMap("hello_world_with_punctuation.txt");
	EXPECT_EQ(wordCountMap["hello"], 1);
	EXPECT_EQ(wordCountMap["world"], 1);
}

TEST(FileAccessManagerTest, ReadFileWithRepeatedWords) {
	FileAccessManager fileMgr;
	map<string, int> wordCountMap = fileMgr.getWordCountMap("hello_world_repeat.txt");
	EXPECT_EQ(wordCountMap["hello"], 3);
	EXPECT_EQ(wordCountMap["world"], 3);
}

TEST(FileAccessManagerTest, ReadCaseSensitiveFile) {
	FileAccessManager fileMgr;
	map<string, int> wordCountMap = fileMgr.getWordCountMap("hello_world_case_sensitive.txt");
	EXPECT_EQ(wordCountMap["hello"], 1);
	EXPECT_EQ(wordCountMap["Hello"], 1);
	EXPECT_EQ(wordCountMap["HELLO"], 1);
	EXPECT_EQ(wordCountMap["hellO"], 1);
}

TEST(IniAccessManagerTest, Load) {
	IniAccessManager iniMgr;
	iniMgr.load("test.ini.txt");
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}



WordCounter::WordCounter()
{	
}


WordCounter::~WordCounter()
{	
}


void WordCounter::load(string filename) {
	this->wordCountMap = this->fileAccessMgr->getWordCountMap(filename);
}

int WordCounter::query(string str) const {	
	map<string, int>::const_iterator it;
	if ((it = this->wordCountMap.find(str)) != this->wordCountMap.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}