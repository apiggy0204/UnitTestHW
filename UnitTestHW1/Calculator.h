#pragma once
class Calculator
{
public:
	Calculator();
	~Calculator();
	int add(int x, int y)      { return x + y; }
	int substact(int x, int y) { return x - y; }
	int multiply(int x, int y) { return x * y; }
	int divide(int x, int y)   { return x / y; }
};

