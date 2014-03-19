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

int TestableCalculator::genRandom() {
	return this->random;
}