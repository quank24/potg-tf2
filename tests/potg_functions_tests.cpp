/*
 * Reinoj, 2019
 */

#include "gtest/gtest.h"
#include "../cpp_src/potg_functions.hpp"

TEST(TimeTests, TimeToSeconds) {
	std::string timeFirst("00:00:00"),
		timeMid("12:34:56"),
		timeLast("23:59:59");
	
	EXPECT_EQ(potg::time_to_seconds(timeFirst), 0);
	EXPECT_EQ(potg::time_to_seconds(timeMid), 45'296);
	EXPECT_EQ(potg::time_to_seconds(timeLast), 86'399);
}
