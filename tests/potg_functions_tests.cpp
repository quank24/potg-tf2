/*
 * Reinoj, 2019
 */

#include "gtest/gtest.h"
#include "../cpp_src/potg_functions.hpp"

TEST(TimeTests, TimeToSeconds) {
	std::string str_times[4] = {
		"L 03/10/2019 - 00:00:00: ...",
		"L 03/10/2019 - 00:00:30: ...",
		"L 03/10/2019 - 00:09:30: ...",
		"L 03/10/2019 - 23:59:59: ..."
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
	std::string lines[3] = {
		"L 03/10/2019 - 20:41:36: \"Cookie<51><[U:1:95932440]><Red>\" triggered \"damage\" against \"only pros can crouch jump<39><[U:1:84375638]><Blue>\" (damage \"5\") (weapon \"flamethrower\")",
		"L 03/10/2019 - 20:41:40: \"i just peed inside your girl<52><[U:1:171261081]><Red>\" triggered \"healed\" against \"Frank from Rhode Island<48><[U:1:133069829]><Red>\" (healing \"29\")",
		"L 03/10/2019 - 20:42:09: \"i just peed inside your girl<52><[U:1:171261081]><Red>\" triggered \"medic_death_ex\" (uberpct \"3\")"
	};
	std::size_t line_indeces[3] = { 149, 171, 119 };
	std::string expected_ret_val[3] = { "5", "29", "3" };
	
	for (int i=0; i<3; ++i) {
		EXPECT_EQ(potg::get_num(lines[i], line_indeces[i]), expected_ret_val[i]);
	}
}

TEST(ParseTests, GetName) {
	std::string line = "L 03/10/2019 - 20:41:36: \"Cookie<51><[U:1:95932440]><Red>\" triggered \"damage\" against \"only pros can crouch jump<39><[U:1:84375638]><Blue>\" (damage \"5\") (weapon \"flamethrower\")";
	
	EXPECT_EQ(potg::get_name(line), "Cookie");
}

TEST(ParseTests, CalculatePoints) {
	std::string lines[9] = {
		"L 03/10/2019 - 20:43:19: \"flexasaurus<45><[U:1:80409878]><Red>\" killed \"Significant Otter<38><[U:1:64705210]><Blue>\" with \"scattergun\" (attacker_position \"-724 -1168 -20\") (victim_position \"-831 -388 -228\")", // killed
		"L 03/10/2019 - 20:43:36: \"Jt<5><[U:1:89587820]><Blue>\" triggered \"kill assist\" against \"flexasaurus<45><[U:1:80409878]><Red>\" (assister_position \"-1157 -608 -241\") (attacker_position \"-1421 -616 -249\") (victim_position \"-573 -453 -196\")", // kill assist
		"L 05/20/2018 - 02:12:20: \"ethansito<7><[U:1:49734297]><Red>\" triggered \"jarate_attack\" against \"Carbon8ed<17><[U:1:228222613]><Blue>\" with \"tf_weapon_jar\" (attacker_position \"2422 -937 411\") (victim_position \"1488 -1573 166\")", // jarate_attack
		"L 05/20/2018 - 02:12:27: \"ScottLuck<10><[U:1:97187490]><Blue>\" triggered \"killedobject\" (object \"OBJ_SENTRYGUN\") (weapon \"pda_engineer\") (objectowner \"ScottLuck<10><[U:1:97187490]><Blue>\") (attacker_position \"1455 -1580 128\")", // killedobject
		"L 05/20/2018 - 02:12:25: \"Floppy Bananas<15><[U:1:282746388]><Red>\" triggered \"healed\" against \"xXDJVinylScratchXx<5><[U:1:97967427]><Red>\" (healing \"34\")", // healed
		"L 05/20/2018 - 02:12:37: \"xXDJVinylScratchXx<5><[U:1:97967427]><Red>\" triggered \"damage\" against \"Catfish<19><[U:1:196217395]><Blue>\" (damage \"19\") (weapon \"obj_sentrygun\")", // damage
		"L 05/20/2018 - 02:12:52: \"IWANNAFUCKURBESTFRIEND<18><[U:1:208757376]><Red>\" triggered \"medic_death\" against \"snek eatur<20><[U:1:119864368]><Blue>\" (healing \"2296\") (ubercharge \"0\")", // medic_death
		"L 05/20/2018 - 02:12:52: \"snek eatur<20><[U:1:119864368]><Blue>\" triggered \"medic_death_ex\" (uberpct \"76\")", // medic_death_ex non-full uber
		"L 05/20/2018 - 02:12:52: \"snek eatur<20><[U:1:119864368]><Blue>\" triggered \"medic_death_ex\" (uberpct \"100\")" // medic_death_ex full uber
	};
	
	std::size_t desc_start[9] = {
		64, 66, 72, 74, 79, 81, 87, 76, 76
	};
	
	double points[9] = {
		25, 10, 2, 5, 34, 38, -5, 3.8, 10
	};
	
	std::string names[9] = {
		"", "", "", "", "", "", "IWANNAFUCKURBESTFRIEND", "", ""
	};
	
	for (size_t i=0; i<9; ++i) {
		std::tuple<double, std::string> t = potg::calculate_points(lines[i], desc_start[i]);
		if (i < 7) {
			EXPECT_EQ(std::get<0>(t), points[i]);
			// thank you ints for being easily represented in binary
		} else {
			EXPECT_TRUE( (std::get<0>(t) > (points[i]-0.01)) && (std::get<0>(t) < (points[i]+0.01)) );
			// gotta love that floating point number precision
		}
		EXPECT_EQ(std::get<1>(t), names[i]);
	}
}

TEST(DescriptorTests, InVector) {
	std::vector<potg::PlayerStats> psv;
	std::string test_name = "Cookie";
	
	EXPECT_EQ(potg::in_vector(psv, test_name), std::string::npos);
	// looking in empty vector
	
	psv.push_back(potg::PlayerStats("Alpha"));
	psv.push_back(potg::PlayerStats("Beta"));
	
	EXPECT_EQ(potg::in_vector(psv, test_name), std::string::npos);
	// looking in non-empty vector without the name
	
	psv.push_back(potg::PlayerStats("Cookie"));
	
	EXPECT_EQ(potg::in_vector(psv, test_name), 2);
	// looking in non-empty vector with the name
}

TEST(DescriptorTests, DescriptorInLine) {
	std::vector<potg::PlayerStats> psv;
	potg::PlayerInfo pi;
	potg::DriverInfo di("L 05/20/2018 - 02:10:04: World triggered \"Round_Start\"", false, "", psv, pi);
	
	descriptor_in_line(di);
	EXPECT_EQ(di.round_in_progress, true);
}
