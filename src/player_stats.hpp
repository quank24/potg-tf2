#ifndef PLAYER_STATS_HPP
#define PLAYER_STATS_HPP

namespace potg {
	struct PlayerStats {
		PlayerStats() {
			name = "";
			// ten_second_deque will start off empty
		}
		PlayerStats(std::string n) : name(n) {}
		PlayerStats(std::string n, std::deque<std::tuple<int,double>> tsd) : name(n), ten_second_deque(tsd) {}
		PlayerStats(const PlayerStats& ps) {
			name = ps.name;
			ten_second_deque = ps.ten_second_deque;
		}
		std::string name;
		std::deque<std::tuple<int, double>> ten_second_deque;
	};
}
#endif  // PLAYER_STATS_HPP
