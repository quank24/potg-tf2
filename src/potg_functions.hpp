#ifndef POTG_FUNCTION_HPP
#define POTG_FUNCTION_HPP

/********** **********
* INCLUDES
********** **********/
#include <cstdlib>  // exit
#include <fstream>  // file i/o
#include <string>  // for string operations

#include "constants.hpp"

/********** **********
* STRUCTS
********** **********/
struct Name {
  bool roundInProgress;
};
/********** **********
* FUNCTIONS
********** **********/
void descriptor_in_line(std::string pLine, bool &pRoundInProgress) {
  if (pRoundInProgress) {
    // something
  } else {
    std::size_t start = pLine.find(WORLD_TRIGGERED);
    if (start != std::string::npos) {
      pRoundInProgress = true;
    }
  }
  return;
  // returns
} // end descriptor_in_line

void foo (std::string pFileName) {
  ifstream fin(file_name.c_str());
  if (fin.fail()) {
    std::cout << "Could not open " << pFileName << ".\n";
    exit(1);
  }
  // if past the fail if-statement, then the file successfully opened.

  std::string line;
  // will store the lines in the log file, one by one
  bool roundInProgress = false;
  while (fin >> line) {
    descriptor_in_line(line, &roundInProgress)
  }
} //

#endif  // POTG_FUNCTION_HPP
