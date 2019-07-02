/*
 * Reinoj, 2019
 */

#ifndef PLAYER_STATS_HPP
#define PLAYER_STATS_HPP

namespace potg {
	struct PlayerStats {
		PlayerStats() {
			name = "";
			// ten_second_queue will start off empty
		}
		PlayerStats(std::string n) : name(n) {}
		PlayerStats(std::string n, std::queue<std::tuple<int,double>> tsq) : name(n), ten_second_queue(tsq) {}
		PlayerStats(const PlayerStats& ps) {
			name = ps.name;
			ten_second_queue = ps.ten_second_queue;
		}
		std::string name;
		std::queue<std::tuple<int, double>> ten_second_queue;
	};
}
#endif  // PLAYER_STATS_HPP
