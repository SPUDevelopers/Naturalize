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

	FactionType GetFactionFromString(std::string name)
	{
		return FACTION_UNKNOWN;
	}

	std::string GetStringFromFaction(FactionType faction)
	{
		return "";
	}

//
//	Movement
//

	enum MoveType
	{
		MOVETYPE_UNKNOWN  = -1,
		MOVETYPE_INFANTRY =  0,
		MOVETYPE_AERIAL,
		MOVETYPE_VEHICLE,
		// ...
		// ^^^ Other definitions go here ^^^
		MOVETYPE_COUNT
	};

	MoveType GetMoveTypeFromString(std::string name)
	{
		return MOVETYPE_UNKNOWN;
	}

	std::string GetStringFromMoveType(MoveType movetype)
	{
		return "";
	}
};

#endif