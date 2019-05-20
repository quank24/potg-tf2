#ifndef TIME_OP_HPP
#define TIME_OP_HPP
#include <iostream>
#include <string>

namespace time_op {
	int time_to_seconds(const std::string& str) {
		// "hh:mm:ss"
		return ( (std::stoi(str.substr(0, 2)) * 360) +
				 (std::stoi(str.substr(3, 2)) * 60) +
				 (std::stoi(str.substr(6, 2))) );
		// hour * 60 * 60 -> seconds
		// minute * 60 -> seconds
	}
	
	int difference(const std::string& lhs, const std::string& rhs) {
		int diff = time_to_seconds(lhs) - time_to_seconds(rhs);
		if (diff < 0) {
			diff += 86'400;
			// 24 * 60 * 60 = 86,400
		}
	}
}


#endif	// TIME_OP_HPP
