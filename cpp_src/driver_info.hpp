/*
 * Reinoj, 2019
 */

#ifndef DRIVER_INFO_HPP
#define DRIVER_INFO_HPP

#include "player_stats.hpp"
#include "player_info.hpp"

namespace potg {
	struct DriverInfo {
		DriverInfo() {
			line = "";
			round_in_progress = false;
			// all_players will start off empty
			best = PlayerInfo();
		}
		DriverInfo(std::string l, bool rip, std::string mk, std::vector<PlayerStats> ap, PlayerInfo b) :
			line(l), round_in_progress(rip), medic_killer(mk), all_players(ap), best(b) {}
		DriverInfo(const DriverInfo& di) {
			line = di.line;
			round_in_progress = di.round_in_progress;
			all_players = di.all_players;
			best = di.best;
		}
		std::string line;
		bool round_in_progress;
		std::string medic_killer;
		std::vector<PlayerStats> all_players;
		PlayerInfo best;
	};
}

#endif  // DRIVER_INFO_HPP
