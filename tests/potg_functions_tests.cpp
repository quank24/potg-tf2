/*
 * Reinoj, 2019
 */
#include <bitset>

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
		"L 03/10/2019 - 20:41:36: \"testname<51><[U:1:95932440]><Red>\" triggered \"damage\" against \"testname<39><[U:1:84375638]><Blue>\" (damage \"5\") (weapon \"flamethrower\")",
		"L 03/10/2019 - 20:41:40: \"testname<52><[U:1:171261081]><Red>\" triggered \"healed\" against \"testname<48><[U:1:133069829]><Red>\" (healing \"29\")",
		"L 03/10/2019 - 20:42:09: \"testname<52><[U:1:171261081]><Red>\" triggered \"medic_death_ex\" (uberpct \"3\")"
	};
	std::size_t line_indeces[3] = { lines[0].find(">\" (damage \"")+12, lines[1].find(">\" (healing \"")+13, lines[2].find("medic_death_ex\" (uberpct \"")+26 };
	std::string expected_ret_val[3] = { "5", "29", "3" };
	
	for (int i=0; i<3; ++i) {
		EXPECT_EQ(potg::get_num(lines[i], line_indeces[i]), expected_ret_val[i]);
	}
}

TEST(ParseTests, GetName) {
	std::string line = "L 03/10/2019 - 20:41:36: \"testname<51><[U:1:95932440]><Red>\" triggered \"damage\" against \"testname<39><[U:1:84375638]><Blue>\" (damage \"5\") (weapon \"flamethrower\")";
	
	EXPECT_EQ(potg::get_name(line), "testname");
}

TEST(ParseTests, CalculatePoints) {
	std::string lines[9] = {
		"L 03/10/2019 - 20:43:19: \"testname<45><[U:1:80409878]><Red>\" killed \"testname<38><[U:1:64705210]><Blue>\" with \"scattergun\" (attacker_position \"-724 -1168 -20\") (victim_position \"-831 -388 -228\")", // killed
		"L 03/10/2019 - 20:43:36: \"testname<5><[U:1:89587820]><Blue>\" triggered \"kill assist\" against \"testname<45><[U:1:80409878]><Red>\" (assister_position \"-1157 -608 -241\") (attacker_position \"-1421 -616 -249\") (victim_position \"-573 -453 -196\")", // kill assist
		"L 05/20/2018 - 02:12:20: \"testname<7><[U:1:49734297]><Red>\" triggered \"jarate_attack\" against \"testname<17><[U:1:228222613]><Blue>\" with \"tf_weapon_jar\" (attacker_position \"2422 -937 411\") (victim_position \"1488 -1573 166\")", // jarate_attack
		"L 05/20/2018 - 02:12:27: \"testname1<10><[U:1:97187490]><Blue>\" triggered \"killedobject\" (object \"OBJ_SENTRYGUN\") (weapon \"pda_engineer\") (objectowner \"testname2<10><[U:1:97187490]><Blue>\") (attacker_position \"1455 -1580 128\")", // killedobject
		"L 05/20/2018 - 02:12:25: \"testname<15><[U:1:282746388]><Red>\" triggered \"healed\" against \"testname<5><[U:1:97967427]><Red>\" (healing \"34\")", // healed
		"L 05/20/2018 - 02:12:37: \"testname<5><[U:1:97967427]><Red>\" triggered \"damage\" against \"testname<19><[U:1:196217395]><Blue>\" (damage \"19\") (weapon \"obj_sentrygun\")", // damage
		"L 05/20/2018 - 02:12:52: \"testname<18><[U:1:208757376]><Red>\" triggered \"medic_death\" against \"testname<20><[U:1:119864368]><Blue>\" (healing \"2296\") (ubercharge \"0\")", // medic_death
		"L 05/20/2018 - 02:12:52: \"testname<20><[U:1:119864368]><Blue>\" triggered \"medic_death_ex\" (uberpct \"76\")", // medic_death_ex non-full uber
		"L 05/20/2018 - 02:12:52: \"testname<20><[U:1:119864368]><Blue>\" triggered \"medic_death_ex\" (uberpct \"100\")" // medic_death_ex full uber
	};
	
	// I can't think of a better way of writing this right now
	std::size_t desc_start[9];
	for (std::size_t i=0; i<8; ++i) {
		desc_start[i] = lines[i].find(potg::POINTS_ARRAY[i]);
	}
	desc_start[8] = lines[8].find(potg::POINTS_ARRAY[7]);
	// separate bc I couldn't think of a way at the moment to do it w/o having it separate.
	
	double points[9] = {
		25, 10, 2, 5, 34, 38, -5, 3.8, 10
	};
	
	std::bitset<9> is_desc(std::string("001000000"));
	// bits are in reverse order, so the first 6 are false, then true, then last 2 are false.
	
	for (size_t i=0; i<9; ++i) {
		std::tuple<double, bool> t = potg::calculate_points(lines[i], desc_start[i]);
		if (i < 7) {
			EXPECT_EQ(std::get<0>(t), points[i]);
			// thank you ints for being easily represented in binary
		} else {
			EXPECT_TRUE( (std::get<0>(t) > (points[i]-0.01)) && (std::get<0>(t) < (points[i]+0.01)) );
			// gotta love that floating point number precision
		}
		EXPECT_EQ(std::get<1>(t), is_desc[i]);
	}
}

TEST(DescriptorTests, InVector) {
	std::vector<potg::PlayerStats> psv;
	std::string test_name = "Gamma";
	
	EXPECT_EQ(potg::in_vector(psv, test_name), std::string::npos);
	// looking in empty vector
	
	psv.push_back(potg::PlayerStats("Alpha"));
	psv.push_back(potg::PlayerStats("Beta"));
	
	EXPECT_EQ(potg::in_vector(psv, test_name), std::string::npos);
	// looking in non-empty vector without the name
	
	psv.push_back(potg::PlayerStats("Gamma"));
	
	EXPECT_EQ(potg::in_vector(psv, test_name), 2);
	// looking in non-empty vector with the name
}

TEST(DescriptorTests, DescriptorInLine) {
	std::vector<potg::PlayerStats> psv;
	potg::PlayerInfo pi;
	potg::DriverInfo di("L 05/20/2018 - 02:24:48: World triggered \"Round_Win\" (winner \"Red\")", true, "", psv, pi);
	
	descriptor_in_line(di);
	EXPECT_EQ(di.round_in_progress, false);
	// check that round in progress gets changed from true to false
	
	di.line = "L 05/20/2018 - 02:10:04: World triggered \"Round_Start\"";
	descriptor_in_line(di);
	EXPECT_EQ(di.round_in_progress, true);
	// check that round in progress gets changed from false to true
	
	di.line = "L 05/20/2018 - 02:23:27: \"testname<17><[U:1:228222613]><Blue>\" triggered \"damage\" against \"quank<3><[U:1:85815930]><Red>\" (damage \"54\") (weapon \"tf_projectile_pipe_remote\")";
	EXPECT_EQ(di.all_players.size(), 0);
	descriptor_in_line(di);
	EXPECT_EQ(di.all_players.size(), 1);
	// check that a PlayerStats was added to the vector
	EXPECT_EQ(di.all_players[0].name, "testname");
	// check that the name was added correctly
	EXPECT_EQ(di.all_players[0].ten_second_queue.size(), 1);
	// check that the element was added to the queue
	EXPECT_EQ(std::get<0>(di.all_players[0].ten_second_queue.front()), 8'607);
	EXPECT_EQ(std::get<1>(di.all_players[0].ten_second_queue.front()), 108);
	// check that the correct values were placed in the tuple in the queue
}
