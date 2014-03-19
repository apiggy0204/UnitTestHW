// UnitTestHW1.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "Calculator.h"
#include "gtest/gtest.h"

TEST(CalculatorTest, Add) {
	Calculator c;
	EXPECT_EQ(3, c.add(1, 2));
}

TEST(CalculatorTest, AddWithRandom) {
	Calculator c;
	c.setRandGenerator(new FakeRandomGeneratorImpl(3));
	EXPECT_EQ(6, c.addWithRandom(1, 2));
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}

