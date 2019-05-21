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
	#include <cstdlib> 	// exit
	#include <fstream>	// file i/o
	#include <queue>	// queue operations
	#include <string>	// string operations
	#include <vector>	// vector operations

	#include "constants.hpp"

	/* -------------------------------------------------------------------------
	 * STRUCTS
	 * -----------------------------------------------------------------------*/
	struct PlayerStats {
		std::queue<std::tuple<int, double>> ten_second_queue;
	};

	struct DriverInfo {
		std::string line;
		bool roundInProgress;
		std::vector 
	};
	/* -------------------------------------------------------------------------
	 * FUNCTIONS
	 * -----------------------------------------------------------------------*/
	/*
	 * will take the string of the time stamp and convert it to an integer
	 * 	number of seconds
	 * parameters:
	 * 	str - time stamp in format "hh:mm:ss"
	 */
	int time_to_seconds(const std::string& str) {
		return ( (std::stoi(str.substr(0, 2)) * 360) +
				 (std::stoi(str.substr(3, 2)) * 60) +
				 (std::stoi(str.substr(6, 2))) );
		// hour * 60 * 60 -> seconds
		// minute * 60 -> seconds
	}
	
	/*
	 * 
	 * parameters:
	 * 	pLine - string of the current line in the file
	 * 	pRoundInProgress - boolean of whether the current line is between the
	 * 		round start and round win lines in the log file
	 */
	void descriptor_in_line(const std::string& pLine, bool &pRoundInProgress) {
	  if (pRoundInProgress) {
		std::size_t win = pLine.find(WORLD_TRIGGERED_WIN);
		if (win != std::string::npos) {
			pRoundInProgress = false;
		} else {
			
		}
		
	  } else {
		std::size_t start = pLine.find(WORLD_TRIGGERED_START);
		if (start != std::string::npos) {
		  pRoundInProgress = true;
		}
	  }
	  return;
	}// end descriptor_in_line



	/*
	 * 
	 */
	void driver(std::string pFileName) {
	  ifstream fin(file_name.c_str());
	  if (fin.fail()) {
		std::cout << "Could not open " << pFileName << std::endl;
		exit(1);
	  }
	  // if past the fail if-statement, then the file successfully opened.

	  std::string line;
	  // will store the lines in the log file, one by one
	  bool roundInProgress = false;
	  while (fin >> line) {
		descriptor_in_line(&line, &roundInProgress)
	  }
	}// end driver
}// end namespace potg
#endif  // POTG_FUNCTION_HPP
