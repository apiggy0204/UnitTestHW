// WordCounter.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "WordCounter.h"
#include "FileAccessManager.h"
#include "IniAccessManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

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

TEST(IniAccessManagerTest, load) {
	IniAccessManager iniMgr;
	iniMgr.load("test.ini");
}

TEST(IniAccessManagerTest, getIntWithKey) {
	IniAccessManager iniMgr;
	iniMgr.load("test_int.ini");
	EXPECT_EQ(123, iniMgr.getIntWithKey("key1"));
	EXPECT_EQ(456, iniMgr.getIntWithKey("key2"));
}

TEST(IniAccessManagerTest, getIntWithKey_WillReturnZero_KeyNotExists) {
	IniAccessManager iniMgr;
	iniMgr.load("test_int.ini");
	EXPECT_EQ(0, iniMgr.getIntWithKey("key3"));
}

TEST(IniAccessManagerTest, getStringSetWithKey) {
	IniAccessManager iniMgr;
	iniMgr.load("test_stringset.ini");
	set<string> stringset = iniMgr.getStringSetWithKey("key1");
	bool helloExists = (stringset.find("hello") != stringset.end());
	bool worldExists = (stringset.find("world") != stringset.end());
	bool babyExists = (stringset.find("baby") != stringset.end());
	EXPECT_TRUE(helloExists);
	EXPECT_TRUE(worldExists);
	EXPECT_FALSE(babyExists);
}

TEST(IniAccessManagerTest, getStringSetWithKey_DifferentDelim) {
	IniAccessManager iniMgr;
	iniMgr.load("test_stringset.ini");
	set<string> stringset = iniMgr.getStringSetWithKey("key2", "/");
	bool helloExists = (stringset.find("hello") != stringset.end());
	bool worldExists = (stringset.find("world") != stringset.end());
	bool babyExists = (stringset.find("baby") != stringset.end());
	EXPECT_TRUE(helloExists);
	EXPECT_TRUE(worldExists);
	EXPECT_FALSE(babyExists);
}

TEST(IniAccessManagerTest, getStringSetWithKey_WillReturnEmpty) {
	IniAccessManager iniMgr;
	iniMgr.load("test_stringset.ini");
	set<string> stringset = iniMgr.getStringSetWithKey("key3");
	bool helloExists = (stringset.find("hello") != stringset.end());
	bool worldExists = (stringset.find("world") != stringset.end());
	EXPECT_FALSE(helloExists);
	EXPECT_FALSE(worldExists);
}

TEST(IniAccessManagerTest, getBool) {
	IniAccessManager iniMgr;
	iniMgr.load("test_bool.ini");
	EXPECT_TRUE(iniMgr.getBoolWithKey("truekey"));
	EXPECT_FALSE(iniMgr.getBoolWithKey("falsekey"));
}

TEST(IniAccessManagerTest, getBool_WillReturnFalse_KeyNotExists) {
	IniAccessManager iniMgr;
	iniMgr.load("test_bool.ini");
	EXPECT_FALSE(iniMgr.getBoolWithKey("notexistentkey"));
}

class MockIniAccessManager : public IniAccessManager {
public:
	MOCK_METHOD1(load, void(string));
	MOCK_CONST_METHOD1(getIntWithKey, int(string keyName));
	MOCK_CONST_METHOD2(getStringSetWithKey, set<string>(string keyName, string delim));
	MOCK_CONST_METHOD1(getBoolWithKey, bool(string keyName));
};

class MockFileAccessManager : public FileAccessManager {
public:
	MOCK_METHOD1(getWordCountMap, map<string, int>(string filename));	
};

/*
class MockManagerFactory : public ManagerFactory {
public:
	MockManagerFactory() {}
	~MockManagerFactory() {}
	FileAccessManager *getFileAccessManager() const { return new MockFileAccessManager; }
	IniAccessManager *getIniAccessManager() const { return new MockIniAccessManager; }
};*/

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}

WordCounter::WordCounter(/*const ManagerFactory& factory*/)
{
	ManagerFactory factory;
	this->fileAccessMgr = factory.getFileAccessManager();
	this->iniAccessMgr = factory.getIniAccessManager();
	this->minWordLength = 0;
	this->hasMinWordLength = false;
	this->isCaseSensitive = false;	
}

WordCounter::~WordCounter()
{
	delete this->fileAccessMgr;
	delete this->iniAccessMgr;
}

void WordCounter::setFileAccessManager(FileAccessManager *fileMgr) {
	if (this->fileAccessMgr) { delete this->fileAccessMgr; }
	this->fileAccessMgr = fileMgr;
}

void WordCounter::setIniAccessManager(IniAccessManager *iniMgr) {
	if (this->iniAccessMgr) { delete this->iniAccessMgr; }
	this->iniAccessMgr = iniMgr;
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

void WordCounter::loadIni(string filename) {
	this->isCaseSensitive = this->iniAccessMgr->getBoolWithKey("isCaseSensitive");
	this->minWordLength = this->iniAccessMgr->getIntWithKey("minWordLength");	
	this->hasMinWordLength = (this->minWordLength > 0) ? true : false;
	this->excludedWordList = this->iniAccessMgr->getStringSetWithKey("excludedWordList");
}



FileAccessManager *ManagerFactory::getFileAccessManager() const {
	return new FileAccessManager;
}
IniAccessManager *ManagerFactory::getIniAccessManager() const {
	return new IniAccessManager;
}