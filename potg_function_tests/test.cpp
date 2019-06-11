#include "pch.h"
#include "gtest/gtest.h"
#include "../cpp_src/potg_functions.hpp"

TEST(TimeFunctionTests, TimeToSeconds) {
	EXPECT_EQ(potg::time_to_seconds("00:00:00"), 0);
	EXPECT_EQ(potg::time_to_seconds("12:18:50"), 44'330);
	// 12*3600 = 43,200 | 18*60 = 1,080
	// 43,200 + 1,080 + 50 = 44,330
	EXPECT_EQ(potg::time_to_seconds("23:59:59"), 86'399);
	// 23*3600 = 82,800 | 59*60 = 3,540
	// 82,800 + 3,540 + 59 = 86,399
}
