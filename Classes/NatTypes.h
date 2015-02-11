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
		MOVETYPE_FLOATING,
		// ...
		// ^^^ Other definitions go here ^^^
		MOVETYPE_COUNT
	};

	MoveType GetMoveTypeFromString(std::string name);
	std::string GetStringFromMoveType(MoveType movetype);

//
//	Attack
//

	//The attack type represents the kind of damage a unit deals
	//This is compared to a unit's defense type to multiply or mitigate incoming damage
	enum AttackType
	{
		ATKTYPE_UNKNOWN = -1,
		ATKTYPE_NORMAL = 0,
		ATKTYPE_FIRE,
		ATKTYPE_POISON,
		ATKTYPE_CARNI,
		ATKTYPE_EXPLODE,
		// ...
		// ^^^ Other definitions go here ^^^
		ATKTYPE_COUNT
	};

	AttackType GetAtkTypeFromString(std::string name);
	std::string GetStringFromAtkType(AttackType movetype);

//
//	Defense
//

	enum DefenseType
	{
		DEFTYPE_UNKNOWN = -1,
		DEFTYPE_NORMAL = 0,
		DEFTYPE_ARMOR,
		DEFTYPE_FLYING,
		DEFTYPE_PLANT,
		// ...
		// ^^^ Other definitions go here ^^^
		DEFTYPE_COUNT
	};

	DefenseType GetDefTypeFromString(std::string name);
	std::string GetStringFromDefType(DefenseType deftype);

};

#endif
