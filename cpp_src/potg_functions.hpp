/*
 * 
 */
void foo() {}


#ifndef POTG_FUNCTION_HPP
#define POTG_FUNCTION_HPP

/* -----------------------------------------------------------------------------
 * INCLUDES
 * ---------------------------------------------------------------------------*/
#include <cstdlib> 	// exit
#include <fstream>	// file i/o
#include <queue>	// queue operations
#include <string>	// string operations
#include <vector>	// vector operations

#include "constants.hpp"

/* -----------------------------------------------------------------------------
 * STRUCTS
 * ---------------------------------------------------------------------------*/
struct PlayerStats {
	std::queue<std::tuple<std::
};

struct DriverInfo {
	std::string line;
	bool roundInProgress;
	std::vector 
};
/* -----------------------------------------------------------------------------
 * FUNCTIONS
 * ---------------------------------------------------------------------------*/
/*
 * parameters:
 * 	pLine - string of the current line in the file
 * 	pRoundInProgress - boolean of whether the current line is between the round
 * 		start and round win lines in the log file
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
  // returns
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

#endif  // POTG_FUNCTION_HPP
