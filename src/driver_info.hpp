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
			round_in_progress = false;
			line = "";
			medic_killer = "";
			name_index = 0;
			time_of_play = 0;
			// all_players will start off empty
			best = PlayerInfo();
		}
		DriverInfo(bool rip, std::string l, std::string mk, std::size_t ni, std::size_t top, std::vector<PlayerStats> ap, PlayerInfo b) :
			round_in_progress(rip), line(l), medic_killer(mk), name_index(ni), time_of_play(top), all_players(ap), best(b) {}
		DriverInfo(const DriverInfo& di) {
			round_in_progress = di.round_in_progress;
			line = di.line;
			medic_killer = di.medic_killer;
			name_index = di.name_index;
			time_of_play = di.time_of_play;
			all_players = di.all_players;
			best = di.best;
		}
		bool round_in_progress;
		std::string line;
		std::string medic_killer;
		std::size_t name_index;
		std::size_t time_of_play;
		std::vector<PlayerStats> all_players;
		PlayerInfo best;
	};
}

#endif  // DRIVER_INFO_HPP
