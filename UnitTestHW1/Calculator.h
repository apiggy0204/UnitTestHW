#pragma once

class RandomGenerator;

class Calculator {
public:
	Calculator();
	~Calculator();
	int add(int x, int y);
	int addWithRandom(int x, int y);

protected:
	virtual int genRandom();
};

class TestableCalculator : public Calculator {
public:
	TestableCalculator() { this->random = 0; }
	void setRandom(int random) { this->random = random; }
protected:
	virtual int genRandom();
	int random;
};