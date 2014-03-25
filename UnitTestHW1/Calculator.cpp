#include "stdafx.h"
#include "Calculator.h"
#include <cstdlib>

Calculator::Calculator() {}

Calculator::~Calculator() {}

int Calculator::add(int x, int y) {
	return x + y;
}

int Calculator::addWithRandom(int x, int y) {
	return x + y + this->randGenerator->genRandom();
}

int Calculator::divide(int x, int y) {
	if (y == 0) {
		this->log->write("Log: Zero divisor");
		throw DivideByZeroException();
	}
	else {
		return x / y;
	}
}

void Calculator::setRandGenerator(RandomGenerator *r) {
	if (this->randGenerator) {
		delete this->randGenerator;
	}
	this->randGenerator = r;
}

void Calculator::setLog(Log *log) {
	this->log = log;
}

int RandomGeneratorImpl::genRandom() {
	return rand();
}

int FakeRandomGeneratorImpl::genRandom() {
	return this->r;
}
