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

TEST(CalculatorTest, Add) {
	Calculator c;
	EXPECT_EQ(3, c.add(1, 2));
}

TEST(CalculatorTest, AddWithRandom) {
	Calculator c;
	MockRandomGenerator r;
	ON_CALL(r, genRandom()).WillByDefault(Return(3));
	c.setRandGenerator(&r);
	EXPECT_EQ(6, c.addWithRandom(1, 2));
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}

