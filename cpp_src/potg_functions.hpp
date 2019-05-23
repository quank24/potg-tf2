/*
 * 
 */
void foo() {}


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
		std::string name;
		std::queue<std::tuple<int, double>> ten_second_queue;
	};

	struct DriverInfo {
		std::string line;
		bool round_in_progress;
		std::vector<PlayerStats> player_stats_vector;
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
		std::string time[3] = {"", "", ""};
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
	}// seconds_to_time
	
	/*
	 * 
	 * parameters:
	 * 	pLine - string of the current line in the file
	 * 	pRoundInProgress - boolean of whether the current line is between the
	 * 		round start and round win lines in the log file
	 */
	void descriptor_in_line(const std::string& pLine, bool &p_round_in_progress) {
	  if (p_round_in_progress) {
		std::size_t win = pLine.find(WORLD_TRIGGERED_WIN);
		if (win != std::string::npos) {
			p_round_in_progress = false;
		} else {
			// look for points
		}
	  } else {
		std::size_t start = pLine.find(WORLD_TRIGGERED_START);
		/* gets the starting index if it is in the line, otherwise it is
		 *  string::npos (the maximum value for size_t */
		if (start != std::string::npos) {
		  p_round_in_progress = true;
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
	  // will store the lines in the log file, one by one
	  bool round_in_progress = false;
	  while (fin >> line) {
		descriptor_in_line(&line, &round_in_progress)
	  }
	}// end driver
}// end namespace potg
#endif  // POTG_FUNCTION_HPP
