/*
 * Reinoj, 2019
 */
 
#ifndef POTG_FUNCTION_HPP
#define POTG_FUNCTION_HPP

namespace potg {
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

	#include "constants.hpp"

	/* -------------------------------------------------------------------------
	 * STRUCTS
	 * -----------------------------------------------------------------------*/
	struct PlayerStats {
		PlayerStats(std::string n, std::queue<std::tuple<int,double>> tsq) {
			name = n;
			ten_second_queue = tsq;
		}
		std::string name;
		std::queue<std::tuple<int, double>> ten_second_queue;
	};

	struct PlayerInfo {
		PlayerInfo(std::string n, int t, double p) {
			name = n;
			time = t;
			points = p;
		}
		std::string name;
		int time;
		double points;
	};
	
	struct DriverInfo {
		DriverInfo(std::string l, bool rip, std::vector<PlayerStats> ap, PlayerInfo b) {
			line = l;
			round_in_progress = rip;
			all_players = ap;
			best = b;
		}
		std::string line;
		bool round_in_progress;
		std::vector<PlayerStats> all_players;
		PlayerInfo best;
		
	};
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
		return ( (std::stoi(str.substr(0, 2)) * 360) +
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
	void seconds_to_time(int seconds) {
		std::string time[3];
		int count = 0;
		std::stringstream ret_str
		while (seconds >= 60) {
			int tmp = seconds % 60;
			seconds /= 60;
			ret_str << setw(2) << std::setfill('0') << std::to_string(tmp);
			/* adds a 0 to the front of the string if it is only one
			 * character */
			time[count] = ret_str.str();
			// set the hours, minutes, or seconds to the string
			count++;
			ret_str(std::string())
			// clear stringstream
		}
		return time[0] + ":" + time[1] + ":" + time[2];
	}// end seconds_to_time
	
	/*
	 * description:
	 * 	gets an unknown length number in a string to an double
	 * parameters:
	 * 	line: the line with a damage or heal value
	 * 	index: integer index of the first number in the damage or healing amount
	 */
	double damage_heal_amount(const std::string& line, std::size_t& index) {
		std::string amount_str = "";
		while (line[index] != "\"") {
			amount_str += line[index];
			index++;
		}
		return amount_str;
	}
	
	/*
	 * description:
	 * 	returns a double for the number of points that the current line was worth. 
	 * parameters:
	 * 	line: current line from the log file
	 * 	descriptor_start: the index where the description starts
	 */
	double calculate_points(const std::string& line, const std::size_t& descriptor_start) {
		std::string descriptor = "";
		int index = descriptor_start;
		while (line[index] != "\"") {
			descriptor += line[index];
		}
		double value = POINTS[descriptor];
		switch(descriptor) {
			case "killed":
			case "kill assist":
			case "jarate_attack":
			case "killedobject":
				return value;
			case "healed":
				std::size_t heal_index = line.find(">\" (healing \"");
				heal_index += 13;
				// first index of the heal amount
				return std::stoi(damage_heal_amount(line, heal_index));
			case "damage":
				std::size_t damage_index = line.find(">\" (damage \"");
				damage_index += 12;
				// first index of the damage amount
				return 2 * std::stoi(damage_heal_amount(line, damage_index));
			case "medic_death_ex":
				std::size_t medic_index = line.find("medic_death_ex\" (uberpct \"");
				medic_index += 25;
				// first index of the uber percentae when killed
				return POINTS["medic_kill"] +  std::stoi(damage_heal_amount(line, medic_index));
			default:
				cout << descriptor << " is not in the map\n";
				exit(1);
		}
	}// end calculate_points 
	
	/*
	 * 
	 * parameters:
	 * 	di: struct that contains:
	 */
	void descriptor_in_line(DriverInfo& di) {
		if (di.round_in_progress) {
			if (find_descriptor(potg::WORLD_TRIGGERED_WIN, line) != std::string::npos) {
				di.round_in_progress = false;
			} else {
				for (auto& map_element : potg::POINTS) {
					std::size_t descriptor_start = line.find(map_element->first);
					if (descriptor_start != std::string::npos) {
						// something = calculate_points(di.line, descriptor_start);
						break;
						/* leave the for loop since we don't need to look for a
						 * 	match anymore */
					}
				}//end for
			}
		} else {
			std::size_t start = di.line.find(potg::WORLD_TRIGGERED_START);
			/* gets the starting index if it is in the line, otherwise it is
			 *  string::npos (the maximum value for size_t */
			if (start != std::string::npos) {
			  di.round_in_progress = true;
			}
		}
	  return;
	}// end descriptor_in_line

	/*
	 * 
	 */
	void driver(std::string p_file_name) {
		ifstream fin(p_file_name.c_str());
		if (fin.fail()) {
			std::cout << "Could not open " << p_file_name << std::endl;
			exit(1);
		}
		// if past the fail if-statement, then the file successfully opened.

		std::string line;
		// stores the lines in the log file, one by one
		DriverInfo di("", false, std::vector<PlayerStats>, PlayerInfo("none", 0), );
		// object that will be passed to the descriptor function
		while (fin >> line) {
			di.line = line;
			descriptor_in_line(di);
		}
	}// end driver
}// end namespace potg
#endif // POTG_FUNCTION_HPP
