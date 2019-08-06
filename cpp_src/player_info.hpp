/*
 * Reinoj, 2019
 */

#ifndef PLAYER_INFO_HPP
#define PLAYER_INFO_HPP

namespace potg {
	struct PlayerInfo {
		PlayerInfo() {
			name = "";
			time = -1;
			points = -1;
		}
		PlayerInfo(std::string n, int t, double p) : name(n), time(t), points(p) {}
		PlayerInfo(const PlayerInfo& pi) {
			name = pi.name;
			time = pi.time;
			points = pi.points;
		}
		std::string name;
		int time;
		double points;
	};
}

#endif  // PLAYER_INFO_HPP
