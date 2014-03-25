// UnitTestHW1.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "Calculator.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;

class MockRandomGenerator : public RandomGenerator {
public:
	MOCK_METHOD0(genRandom, int());
};

class MockLog : public Log {
public:
	MOCK_METHOD1(write, void(string));
};

TEST(CalculatorTest, add) {
	Calculator c;
	EXPECT_EQ(3, c.add(1, 2));
}

TEST(CalculatorTest, addWithRandom) {
	Calculator c;
	MockRandomGenerator r;
	ON_CALL(r, genRandom()).WillByDefault(Return(3));
	c.setRandGenerator(&r);
	EXPECT_EQ(6, c.addWithRandom(1, 2));
}

TEST(CalculatorTest, divide_DivideByNonzero) {
	Calculator c;
	EXPECT_EQ(3, c.divide(10, 3));
}

TEST(CalculatorTest, divide_DivideByZero_LogCalledOnce) {
	Calculator c;
	MockLog mockLog;
	c.setLog(&mockLog);
	EXPECT_CALL(mockLog, write("Log: Zero divisor")).Times(1);
	try {
		c.divide(8, 0);
	}
	catch (...) {}
}

TEST(CalculatorTest, divide_DivideByZero_ThrowsDivideByZeroException) {
	Calculator c;
	MockLog mockLog;
	c.setLog(&mockLog);
	EXPECT_THROW(c.divide(8, 0), DivideByZeroException);
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}

