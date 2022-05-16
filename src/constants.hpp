#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>

namespace potg {
	const std::string WORLD_TRIGGERED_START = "World triggered \"Round_Start\"";

	const std::string WORLD_TRIGGERED_WIN = "World triggered \"Round_Win\"";

	std::string POINTS_ARRAY[8] = {
		"killed \"", "kill assist\"", "jarate_attack\"", "killedobject\"", "healed\"", "damage\"", "medic_death\"", "medic_death_ex\""
	};

	std::map<std::string, double> POINTS = {
		{"killed \"", 25},  // extra space after killed because it is the only one not surrounded by quotes
		{"kill assist\"", 10},
		{"jarate_attack\"", 2},
		{"killedobject\"", 5},
		{"healed\"", 1},
		{"damage\"", 2},
		{"medic_death\"", -5}, // killing a medic will be worth less and there's also a line for killed, so this subtracts 5 from the "killed" value
		{"medic_death_ex\"", 0.05}
	};
	
	std::map<std::string, int> SWITCH_VALUE = {
		{"killed ", 0},
		{"kill assist", 1},
		{"jarate_attack", 2},
		{"killedobject", 3},
		{"healed", 4},
		{"damage", 5},
		{"medic_death", 6},
		{"medic_death_ex", 7}
	};
}// end namespace potg

#endif // CONSTANTS_H
