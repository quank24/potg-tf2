#ifndef POTG_FUNCTION_HPP
#define POTG_FUNCTION_HPP

/* -------------------------------------------------------------------------
 * INCLUDES
 * -----------------------------------------------------------------------*/
#include <cstdlib> 	// exit, size_t
#include <deque>	// deque operations
#include <fstream>	// file i/o
#include <iomanip>	// std::setw(int), std::setfill('0')
#include <iostream>
#include <sstream>	// std::stringstream, .str()
#include <string>	// string operations
#include <vector>	// vector operations
#include <typeinfo>	// i don't remember why i included this
#include "constants.hpp"
#include "player_stats.hpp"
#include "player_info.hpp"
#include "driver_info.hpp"

namespace potg {
	/* -------------------------------------------------------------------------
	 * FUNCTIONS
	 * -----------------------------------------------------------------------*/
	/*
	 * takes the line and converts the time to an integer number of seconds.
	 * parameters:
	 * 	line - a line from the log files
	 */
	int time_to_seconds(const std::string& line) {
		return ( (std::stoi(line.substr(15, 2)) * 3600) +
				 (std::stoi(line.substr(18, 2)) * 60) +
				 (std::stoi(line.substr(21, 2))) );
		// hour * 60 * 60 -> seconds
		// minute * 60 -> seconds
	}// end time_to_seconds
	
	/*
	 * takes the integer number of seconds and converts it to a string formatted
	 * 	in the time format the log file uses.
	 * parameters:
	 * 	seconds - converted number of seconds from a time stamp
	 */
	std::string seconds_to_time(int seconds) {
		int time[3] = {0, 0, 0};
		// initialize the values to 0
		
		time[0] = seconds/3600;
		// gets hours
		if (time[0]) {
			seconds -= (3'600 * time[0]);
			// takes the hours off of the total
		}
		
		time[1] = seconds/60;
		// gets minutes
		if (time[1]) {
			seconds -= (60 * time[1]);
			// takes the minutes off of the total
		}
		
		time[2] = seconds;
		// assigns remaining to seconds
		
		std::stringstream ret_str;
		// formatting the string
		for (int i=0; i<3; ++i) {
			ret_str << std::setw(2) << std::setfill('0') << std::to_string(time[i]);
			// want the number to be 2 characters wide with leading 0's if necessary
			if (i<2) {
				ret_str << ":";
				// only have colons between hours:minutes and minutes:seconds
			}
		}
		return ret_str.str();
		// returns the stringstream as a string
	}// end seconds_to_time
	
	/*
	 * description:
	 * 	gets an unknown length number in a string
	 * parameters:
	 * 	line: the line with a damage, heal, or medic_death value - " ...###"... "
	 * 	index: integer index of the first number in the damage or healing amount
	 */
	std::string get_num(const std::string& line, std::size_t& index) {
		std::string amount_str = "";
		while (line[index] != '\"') {
			amount_str += line[index];
			index++;
		}
		return amount_str;
	}
	
	/*
	 * 
	 */
	std::string get_name(const std::string& line) {
		std::size_t start = line.find("\"") + 1;
		// gets the index of the first character of the steam name
		if (start == std::string::npos) {
			std::cout << "no \" in the line.\n" << line << std::endl;
			exit(1);
		} else {
			std::size_t end = line.find("><[U:");
			if (end == std::string::npos) {
				std::cout << "no ><[U: in the line.\n" << line << std::endl;
				exit(1);
			}
			
			while (line[end] != '<') {
				end--;
			} // gets the index right after the last character of the steam name
			return line.substr(start, end-start); // start index, length of the substring
		}
	}
	
	
	/*
	 * description:
	 * 	returns a double for the number of points that the current line was worth and
	 *  a bool of whether to save the name to the med_killer variable of DriverInfo.
	 * parameters:
	 * 	line: current line from the log file
	 * 	descriptor_start: the index where the description starts
	 */
	std::tuple<double, bool> calculate_points(const std::string& line, const std::size_t& descriptor_start) {
		std::string descriptor("");
		bool med_killer(false);
		int index = descriptor_start;
		while (line[index] != '\"') {
			descriptor += line[index];
			index++;
		}
		int descriptor_value = SWITCH_VALUE[descriptor];
		// have to get this value before I add the quotation mark
		descriptor += "\"";
		// have to add the quotation at the end bc of the dumbass way I've set things up.
		double value = POINTS[descriptor];
		switch(descriptor_value) {
			case 0: // killed
			case 1: // kill assist
			case 2: // jarate_attack
			case 3: // killedobject
				// do nothing because we just want the value, the rest scale
				break;
			case 4: // healed
				{
					std::size_t heal_index = line.find(">\" (healing \"");
					heal_index += 13;
					// first index of the heal amount
					value = std::stoi(get_num(line, heal_index));
				}
				break;
			case 5: // damage
				{
					std::size_t damage_index = line.find(") (realdamage \"");
					if (damage_index == std::string::npos) {
						damage_index = line.find(">\" (damage \"");
						damage_index += 12;
						// first index of the damage amount
					} else {
						damage_index += 15;
						// first index of the real damage amount
					}
					value *= std::stoi(get_num(line, damage_index));
				}
				break;
			case 6: // medic_death
				{
					med_killer = true;
				}
				break;
			case 7: // medic_death_ex
				{
					std::size_t medic_index = line.find("medic_death_ex\" (uberpct \"");
					medic_index += 26;
					// first index of the uber percentae when killed
					int uberpct = std::stoi(get_num(line, medic_index));
					value *= uberpct;
					value += ((uberpct/100) * (-1 * POINTS["medic_death\""]));
					// integer division, so if the uber was full it'll equal 1
					// then it multiplies the negative number from medic death by -0.5, as a bonus for killing a medic with full uber
					// so with a full uber it would be 100*0.05 = 5 and 1*-1*-5 = 5, which would bring the total up to 30
					// ^^ when kills=25, medic_death=-5, medic_death_ex=0.05
				}
				break;
			default:
				std::cout << descriptor << " is not in the map\n";
				exit(1);
		}
		return std::make_tuple(value, med_killer);
	}// end calculate_points 
	
	std::size_t in_vector(const std::vector<PlayerStats>& ps, const std::string& name) {
		for (std::size_t i=0; i<ps.size(); ++i) {
			if (ps[i].name == name) {
				return i;
			}
		}
		return std::string::npos;
	}
	
	/*
	 * 
	 * parameters:
	 * 	di: struct that contains:
	 */
	void descriptor_in_line(DriverInfo& di) {
		std::string scorer_name = "";
		//std::size_t name_index(std::string::npos), time_of_play(std::string::npos);
		if (di.round_in_progress) {
			if (di.line.find(WORLD_TRIGGERED_WIN) != std::string::npos) {
				di.round_in_progress = false;
			} else {
				for (auto& map_element : POINTS) {
					std::size_t descriptor_start = di.line.find(map_element.first);
					if (descriptor_start != std::string::npos) {
						// if the descriptor is in the line then
						std::tuple<double, bool> points_med = calculate_points(di.line, descriptor_start);
						// double is the points amount
						// bool is whether the descriptor was medic_death
						di.time_of_play = time_to_seconds(di.line);
						// converts the time at the beginning of the line to an int number of seconds
						scorer_name = get_name(di.line);
						// gets the name of the scorer of the line
						if (di.medic_killer != "") {
							scorer_name = di.medic_killer;
							// give the points to the killer
							di.medic_killer = "";
							// reset the string to empty
						}
						if (std::get<1>(points_med)) {
							di.medic_killer = scorer_name;
							// sets the DriverInfo string to the name if the bool returned was true.
						}
						di.name_index = in_vector(di.all_players, scorer_name);
						if (di.name_index == std::string::npos) {
							// if it returned npos, then that player is not in the queue
							di.all_players.push_back(PlayerStats(scorer_name));
							// adds new element to end of vector
							di.name_index = di.all_players.size()-1;
							// sets the name index to the new vector element
						}
						di.all_players[di.name_index].ten_second_deque.push_back(std::make_tuple(di.time_of_play, std::get<0>(points_med)));
						// adds the tuple to that player's queue
						break;
						// leave the for loop since we don't need to look for a match anymore
					}
				}//end for auto &map_element
			}
		} else {
			std::size_t start = di.line.find(WORLD_TRIGGERED_START);
			// gets the starting index if it is in the line, otherwise it is string::npos (the maximum value for size_t
			if (start != std::string::npos) {
			  di.round_in_progress = true;
			}
		}
	  return;
	}// end descriptor_in_line


	/*
	 * description:
	 * 	iterate through the scorer's deque, pop any old points off, check if it's better than the current max
	 * parameter:
	 * 	di the info passed from the driver with the deque info
	 */
	void update_driver_info(DriverInfo& di) {
		if (di.name_index != std::string::npos) {
			double new_score(0);
			for (std::size_t i=0; i<di.all_players[di.name_index].ten_second_deque.size(); ++i) {
				int difference = di.time_of_play - std::get<0>(di.all_players[di.name_index].ten_second_deque[i]);
				// so it doesn't have to calculate the value twice for the comparisons
				if (difference >= 0 && difference <= 10) {
					new_score += std::get<1>(di.all_players[di.name_index].ten_second_deque[i]);
					// add the points to the running total
				} else {
					di.all_players[di.name_index].ten_second_deque.pop_front();
					// it should only pop off items at the front, if it pops off stuff in the middle then we have a problem
					// might be a problem if the game happens during the change between hour 23 and 0
					--i;
					// one less element, need to offset the increment
				}
			} // end for
			
			if (new_score >= di.best.points) {
				di.best.name = di.all_players[di.name_index].name;
				// store name of the current player
				di.best.time = std::get<0>(di.all_players[di.name_index].ten_second_deque[0]);
				// store the time of the beginning
				di.best.points = new_score;
				// store the point total for that 10 second time
			}
			di.name_index = std::string::npos;
			// reset name_index
		}
	}// end update_driver_info

	/*
	 * 
	 */
	void driver(char* file_name) {
		std::ifstream fin(file_name);
		if (fin.fail()) {
			std::cout << "Could not open " << file_name << std::endl;
			exit(1);
		}
		
		// if past the fail if-statement, then the file successfully opened.

		std::string line;
		// stores the lines in the log file, one by one
		std::vector<PlayerStats> psv;
		PlayerInfo pi;
		DriverInfo di(false, "", "", std::string::npos, std::string::npos, psv, pi);
		// object that will be passed to the descriptor function
		while (getline(fin, line)) {
			di.line = line;
			descriptor_in_line(di);
			if (di.round_in_progress) {
				update_driver_info(di);
			}
		}
		fin.close();
		std::cout << "name: " << di.best.name << "\n"
			<< "time: " << seconds_to_time(di.best.time) << "\n"
			<< "points: " << di.best.points << "\n\n";
	}// end driver

}// end namespace potg
#endif // POTG_FUNCTION_HPP
