// WordCounter.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "WordCounter.h"
#include "FileAccessManager.h"
#include "IniAccessManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;

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

class MockManagerFactory : public ManagerFactory {
public:
	MockManagerFactory() {}
	~MockManagerFactory() {}
	FileAccessManager *getFileAccessManager() const { return new MockFileAccessManager; }
	IniAccessManager *getIniAccessManager() const { return new MockIniAccessManager; }
};

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

TEST(WordCounterTest, query_CaseSensitive) {
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hello", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hello", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.load("test.txt");
	EXPECT_EQ(1, counter.query("hello"));
	EXPECT_EQ(1, counter.query("world"));
	EXPECT_EQ(1, counter.query("Hello"));
	EXPECT_EQ(1, counter.query("worlD"));	
}

TEST(WordCounterTest, query_CaseInsensitive) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hello", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hello", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));	

	// Ini
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(false));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(0));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(set<string>()));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(2, counter.query("hello"));
	EXPECT_EQ(2, counter.query("world"));
	EXPECT_EQ(2, counter.query("Hello"));
	EXPECT_EQ(2, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseSensitive_MinWordLength) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(true));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(4));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(set<string>()));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(0, counter.query("hi"));
	EXPECT_EQ(1, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(1, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseInsensitive_ExcludedWordList) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));	
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	set<string> excludedStrings;	
	excludedStrings.insert("world");
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(false));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(0));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(excludedStrings));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(2, counter.query("hi"));
	EXPECT_EQ(0, counter.query("world"));
	EXPECT_EQ(2, counter.query("Hi"));
	EXPECT_EQ(0, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseInsensitive_ExcludedWordList_MinWordLength) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));	
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	set<string> excludedStrings;
	excludedStrings.insert("HI");	
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(false));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(5));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(excludedStrings));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(0, counter.query("hi"));
	EXPECT_EQ(2, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(2, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseInsensitive_MinWordLength) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(false));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(5));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(set<string>()));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(0, counter.query("hi"));
	EXPECT_EQ(2, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(2, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseSensitive_ExcludedWordList) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	set<string> excludedStrings;
	excludedStrings.insert("Hi");
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(true));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(0));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(excludedStrings));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(1, counter.query("hi"));
	EXPECT_EQ(1, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(1, counter.query("worlD"));
}

TEST(WordCounterTest, query_CaseInsensitive_MinWordLength_ExcludedWordList) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	wordCountMap.insert(pair<string, int>("verylongword", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	set<string> excludedStrings;
	excludedStrings.insert("world");
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(false));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(5));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(excludedStrings));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(0, counter.query("hi"));
	EXPECT_EQ(0, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(0, counter.query("worlD"));
	EXPECT_EQ(1, counter.query("VeryLongWord"));
}

TEST(WordCounterTest, query_CaseSensitive_MinWordLength_ExcludedWordList) {
	// Text file
	MockFileAccessManager *fileMgr = new MockFileAccessManager;
	map<string, int> wordCountMap;
	wordCountMap.insert(pair<string, int>("hi", 1));
	wordCountMap.insert(pair<string, int>("world", 1));
	wordCountMap.insert(pair<string, int>("Hi", 1));
	wordCountMap.insert(pair<string, int>("worlD", 1));
	wordCountMap.insert(pair<string, int>("verylongword", 1));
	wordCountMap.insert(pair<string, int>("VeryLongWord", 1));
	EXPECT_CALL(*fileMgr, getWordCountMap("test.txt")).WillOnce(Return(wordCountMap));

	// Ini
	set<string> excludedStrings;
	excludedStrings.insert("verylongword");
	excludedStrings.insert("world");
	MockIniAccessManager *iniMgr = new MockIniAccessManager;
	EXPECT_CALL(*iniMgr, getBoolWithKey("isCaseSensitive")).WillOnce(Return(true));
	EXPECT_CALL(*iniMgr, getIntWithKey("minWordLength")).WillOnce(Return(5));
	EXPECT_CALL(*iniMgr, getStringSetWithKey("excludedWordList", ";")).WillOnce(Return(excludedStrings));

	// Testing query
	WordCounter counter;
	counter.setFileAccessManager(fileMgr);
	counter.setIniAccessManager(iniMgr);
	counter.loadIni("test.ini");
	counter.load("test.txt");
	EXPECT_EQ(0, counter.query("hi"));
	EXPECT_EQ(0, counter.query("world"));
	EXPECT_EQ(0, counter.query("Hi"));
	EXPECT_EQ(1, counter.query("worlD"));
	EXPECT_EQ(0, counter.query("verylongword"));
	EXPECT_EQ(1, counter.query("VeryLongWord"));
}

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
	this->isCaseSensitive = true;
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

void WordCounter::load(const string& filename) {
	this->wordCountMap = this->fileAccessMgr->getWordCountMap(filename);
}

string toLower(const string& input) {
	string data = input;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

int WordCounter::query(const string& str) const {
	map<string, int>::const_iterator it;
	int ret = 0; 

	for (it = wordCountMap.begin(); it != wordCountMap.end(); ++it) {
		//cout << "toLower: " << toLower(it->first) << " " << toLower(str) << endl;
		if (!this->hasMinWordLength || str.length() >= this->minWordLength) {
			if ((this->isCaseSensitive && it->first == str) || (!this->isCaseSensitive && toLower(it->first) == toLower(str))) {									
				if (!this->isExcludedWord(str))
					ret += it->second;
			}
		}
	}

	return ret;	
}

void WordCounter::loadIni(const string& filename) {
	this->iniAccessMgr->load(filename);
	this->isCaseSensitive = this->iniAccessMgr->getBoolWithKey("isCaseSensitive");
	this->minWordLength = this->iniAccessMgr->getIntWithKey("minWordLength");	
	this->hasMinWordLength = (this->minWordLength > 0) ? true : false;
	this->excludedWordList = this->iniAccessMgr->getStringSetWithKey("excludedWordList");
}

bool WordCounter::isExcludedWord(const string& str) const {	
	set<string>::const_iterator it;
	for (it = this->excludedWordList.begin(); it != this->excludedWordList.end(); ++it) {
		if ((this->isCaseSensitive && *it == str) || (!this->isCaseSensitive && toLower(*it) == toLower(str)))
			return true;
	}

	return false;
}

FileAccessManager *ManagerFactory::getFileAccessManager() const {
	return new FileAccessManager;
}
IniAccessManager *ManagerFactory::getIniAccessManager() const {
	return new IniAccessManager;
}

