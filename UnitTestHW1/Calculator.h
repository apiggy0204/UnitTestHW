#pragma once

class RandomGenerator;

class Calculator
{
public:
	Calculator();
	~Calculator();
	int add(int x, int y);
	int addWithRandom(int x, int y);

	//Property Injection for UT
	void setRandGenerator(RandomGenerator *r);

protected:
	//virtual int genRandom();
	RandomGenerator *randGenerator = 0;
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
