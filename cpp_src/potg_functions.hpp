/*
 * Reinoj, 2019
 */
 
#ifndef POTG_FUNCTION_HPP
#define POTG_FUNCTION_HPP

/* -------------------------------------------------------------------------
 * INCLUDES
 * -----------------------------------------------------------------------*/
#include <cstdlib> 	// exit, size_t
#include <fstream>	// file i/o
#include <iomanip>	// std::setw(int), std::setfill('0')
#include <queue>	// queue operations
#include <sstream>	// std::stringstream, .str()
#include <string>	// string operations
#include <vector>	// vector operations
#include <typeinfo>
#include "constants.hpp"
#include "player_stats.hpp"
#include "player_info.hpp"
#include "driver_info.hpp"

namespace potg {
	/* -------------------------------------------------------------------------
	 * FUNCTIONS
	 * -----------------------------------------------------------------------*/
	/*
	 * takes the string of the time stamp and converts it to an integer number
	 *  of seconds.
	 * parameters:
	 * 	str - time stamp in format "hh:mm:ss"
	 */
	int time_to_seconds(const std::string& str) {
		return ( (std::stoi(str.substr(0, 2)) * 3600) +
				 (std::stoi(str.substr(3, 2)) * 60) +
				 (std::stoi(str.substr(6, 2))) );
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
	 * 	returns a double for the number of points that the current line was worth. 
	 * parameters:
	 * 	line: current line from the log file
	 * 	descriptor_start: the index where the description starts
	 */
	std::tuple<double, std::string> calculate_points(const std::string& line, const std::size_t& descriptor_start) {
		std::string descriptor(""), med_killer("");
		int index = descriptor_start;
		while (line[index] != '\"') {
			descriptor += line[index];
			index++;
		}
		double value = potg::POINTS[descriptor];
		int descriptor_value = potg::SWITCH_VALUE[descriptor];
		switch(descriptor_value) {
			case 0: // killed
			case 1: // kill assist
			case 2: // jarate_attack
			case 3: // killedobject
				// do nothing because we just want the value, but dont want it to return an error
				// I could probably just not have this and rewrite it, but maybe later
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
					std::size_t damage_index = line.find(">\" (damage \"");
					damage_index += 12;
					// first index of the damage amount
					value *= std::stoi(get_num(line, damage_index));
				}
				break;
			case 6: // medic_death
				{
					med_killer = get_name(line);
				}
				break;
			case 7: // medic_death_ex
				{
					std::size_t medic_index = line.find("medic_death_ex\" (uberpct \"");
					medic_index += 26;
					// first index of the uber percentae when killed
					int uberpct = std::stoi(get_num(line, medic_index));
					value *= uberpct;
					value += ((uberpct/100) * (-1 * potg::POINTS["medic_death"]));
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
	
	/*
	 * 
	 * parameters:
	 * 	di: struct that contains:
	 */
/*	void descriptor_in_line(DriverInfo& di) {
		if (di.round_in_progress) {
			if (find_descriptor(potg::WORLD_TRIGGERED_WIN, line) != std::string::npos) {
				di.round_in_progress = false;
			} else {
				for (auto& map_element : potg::POINTS) {
					std::size_t descriptor_start = line.find(map_element->first);
					if (descriptor_start != std::string::npos) {
						double points;
						std::string med_killer;
						// auto [points, med_killer] = calculate_points(di.line, descriptor_start);
						di.medic_killer = med_killer;
						break;
						// leave the for loop since we don't need to look for a match anymore
					}
				}//end for
			}
		} else {
			std::size_t start = di.line.find(potg::WORLD_TRIGGERED_START);
			// gets the starting index if it is in the line, otherwise it is string::npos (the maximum value for size_t
			if (start != std::string::npos) {
			  di.round_in_progress = true;
			}
		}
	  return;
	}// end descriptor_in_line
*/
	/*
	 * 
	 */
/*	void driver(std::string p_file_name) {
		ifstream fin(p_file_name.c_str());
		if (fin.fail()) {
			std::cout << "Could not open " << p_file_name << std::endl;
			exit(1);
		}
		// if past the fail if-statement, then the file successfully opened.

		std::string line;
		// stores the lines in the log file, one by one
		DriverInfo di("", false, "", std::vector<PlayerStats>, PlayerInfo("none", 0), );
		// object that will be passed to the descriptor function
		while (fin >> line) {
			di.line = line;
			descriptor_in_line(di);
		}
	}// end driver
*/
}// end namespace potg
#endif // POTG_FUNCTION_HPP
