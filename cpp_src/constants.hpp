/*
 * Reinoj, 2019
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace potg {
	const std::string WORLD_TRIGGERED_START = "World triggered \"Round_Start\"";

	const std::string WORLD_TRIGGERED_WIN = "World triggered \"Round_Win\"";

	const std::map<std::string, double> POINTS = {
		{"killed", 25},
		{"kill assist", 10},
		{"jarate_attack", 5},
		{"killedobject", 5},
		{"healed", 1},
		{"damage", 2},
		{"medic_death_ex", 0.02},
		{"medic_kill", 20.0}
	}
}// end namespace potg

#endif // CONSTANTS_H
