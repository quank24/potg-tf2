/*
 * Reinoj, 2019
 */

#include "gtest/gtest.h"
#include "../cpp_src/potg_functions.hpp"

TEST(TimeTests, TimeToSeconds) {
	std::string first("00:00:00"),
		seconds("00:00:30"),
		minutes("00:09:30"),
		last("23:59:59");
	
	EXPECT_EQ(potg::time_to_seconds(first), 0);
	EXPECT_EQ(potg::time_to_seconds(seconds), 30);
	EXPECT_EQ(potg::time_to_seconds(minutes), 570);
	EXPECT_EQ(potg::time_to_seconds(last), 86'399);
}

TEST(TimeTests, SecondsToTime) {
	int first(0),
		seconds(30),
		minutes(570),
		last(86'399);
	EXPECT_EQ(potg::seconds_to_time(first), "00:00:00");
	EXPECT_EQ(potg::seconds_to_time(seconds), "00:00:30");
	EXPECT_EQ(potg::seconds_to_time(minutes), "00:09:30");
	EXPECT_EQ(potg::seconds_to_time(last), "23:59:59");
}

TEST(ParseTests, GetNum) {
	std::string lines[3] = {"L 03/10/2019 - 20:41:36: \"Cookie<51><[U:1:95932440]><Red>\" triggered \"damage\" against \"only pros can crouch jump<39><[U:1:84375638]><Blue>\" (damage \"5\") (weapon \"flamethrower\")",
		"L 03/10/2019 - 20:41:40: \"i just peed inside your girl<52><[U:1:171261081]><Red>\" triggered \"healed\" against \"Frank from Rhode Island<48><[U:1:133069829]><Red>\" (healing \"29\")",
		"L 03/10/2019 - 20:42:09: \"i just peed inside your girl<52><[U:1:171261081]><Red>\" triggered \"medic_death_ex\" (uberpct \"3\")"};
	std::size_t line_indeces[3] = { 149, 171, 119 };
	std::string expected_ret_val[3] = { "5", "29", "3" };
	
	for (int i=0; i<3; ++i) {
		EXPECT_EQ(potg::get_num(lines[i], line_indeces[i]), expected_ret_val[i]);
	}
}
