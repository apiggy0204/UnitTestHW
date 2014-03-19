#include "stdafx.h"
#include "Calculator.h"
#include <cstdlib>

Calculator::Calculator() {}

Calculator::~Calculator() {}

int Calculator::add(int x, int y) {
	return x + y;
}

int Calculator::addWithRandom(int x, int y) {
	return x + y + genRandom();
}

int Calculator::genRandom() {
	return rand();
}

/*
void Calculator::setRandGenerator(RandomGenerator *r) {
	if (this->randGenerator) {
		delete this->randGenerator;
	}
	this->randGenerator = r;
}

int RandomGeneratorImpl::genRandom() {
	return rand();
}

int FakeRandomGeneratorImpl::genRandom() {
	return this->r;
}
*/