// UnitTestHW1.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "Calculator.h"
#include "gtest/gtest.h"

TEST(CalculatorTest, Add) {
	Calculator c;
	EXPECT_EQ(3, c.add(1, 2));
}

TEST(CalculatorTest, Substract) {
	Calculator c;
	EXPECT_EQ(5, c.substact(10, 4));
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}

