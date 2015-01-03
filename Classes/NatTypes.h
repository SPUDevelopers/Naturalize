// NatTypes.h -- Naturalize Type Helpers
// Contains definitions for unit/movement types

#ifndef __NATTYPES_H__
#define __NATTYPES_H__

#include <string>

namespace Naturalize
{
//
//	Factions
//

	enum FactionType
	{
		FACTION_UNKNOWN = -1,
		FACTION_HUMAN	=  0,
		FACTION_PLANT,
		// ...
		// ^^^ Other definitions go here ^^^
		FACTION_COUNT
	};

	FactionType GetFactionFromString(std::string name);
	std::string GetStringFromFaction(FactionType faction);

//
//	Movement
//

	enum MoveType
	{
		MOVETYPE_UNKNOWN  = -1,
		MOVETYPE_INFANTRY =  0,
		MOVETYPE_VEHICLE,
		MOVETYPE_FLYING,
		// ...
		// ^^^ Other definitions go here ^^^
		MOVETYPE_COUNT
	};

	MoveType GetMoveTypeFromString(std::string name);
	std::string GetStringFromMoveType(MoveType movetype);

};

#endif
