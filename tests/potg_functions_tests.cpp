/*
 * Reinoj, 2019
 */

#include "gtest/gtest.h"
#include "../cpp_src/potg_functions.hpp"

TEST(TimeTests, TimeToSeconds) {
	std::string str_times[4] = {
		"00:00:00", "00:00:30", "00:09:30", "23:59:59"
	};
	int int_times[4] = {
		0, 30, 570, 86'399
	};
	
	for (std::size_t i=0; i<4; ++i) {
		EXPECT_EQ(potg::time_to_seconds(str_times[i]), int_times[i]);
	}
}

TEST(TimeTests, SecondsToTime) {
	std::string str_times[4] = {
		"00:00:00", "00:00:30", "00:09:30", "23:59:59"
	};
	int int_times[4] = {
		0, 30, 570, 86'399
	};
	
	for (std::size_t i=0; i<4; ++i) {
		EXPECT_EQ(potg::seconds_to_time(int_times[i]), str_times[i]);
	}
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
