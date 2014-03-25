#pragma once
#include "Log.h"

class RandomGenerator;

class Calculator
{
public:
	Calculator();
	~Calculator();
	int add(int x, int y);
	int addWithRandom(int x, int y);
	int divide(int x, int y);

	//Property Injection for UT
	void setRandGenerator(RandomGenerator *r);
	void setLog(Log *log);

protected:
	RandomGenerator *randGenerator = 0;
	Log *log = 0;
};

class RandomGenerator {
public:
	virtual int genRandom() = 0;
	virtual ~RandomGenerator() {};
};

class RandomGeneratorImpl : public RandomGenerator {
public:
	virtual int genRandom();
	virtual ~RandomGeneratorImpl() {}
};

class FakeRandomGeneratorImpl : public RandomGenerator {
public:
	FakeRandomGeneratorImpl(int r) { this->r = r; }
	virtual int genRandom();
	virtual ~FakeRandomGeneratorImpl() {}
	int r;
};

class DivideByZeroException {};
