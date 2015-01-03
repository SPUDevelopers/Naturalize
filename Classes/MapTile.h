//
//  MapTile.h
//
//  Created by Evan McNulty on 12/21/14.
//

#ifndef __MAPTILE_H__
#define __MAPTILE_H__

#include "cocos2d.h"

#include "NatTypes.h"

using namespace cocos2d;

// The Tile class basically holds a bunch of tile attributes and stat for a
// given type of tile. For example, a field tile may have high visibility
// stat, low movement penalty, but a low defense buff, stuff like that.
// Attributes and methods should follow the Naturalize documentation.
class MapTile
{
public:
    MapTile();
    ~MapTile();
    
    // Returns a value to affect damage done by a unit
    // Based on int[unit.faction] where unit.faction = for human, 1 for plant
    // Values can be negative
	int atkBuf[Naturalize::FACTION_COUNT];
    
    // Returns a value to affect damage done to a unit
    // Based on int[unit.faction] where unit.faction = for human, 1 for plant
    // Values can be negative
	int defBuf[Naturalize::FACTION_COUNT];
    
    // Movement cost for tiles of a certain type int[tile.type]
    // Tile type mapping to be determined, 6 at the moment
    // Movement ability is calculated by unit movement available - moveCost
	int moveCost[Naturalize::MOVETYPE_COUNT];

};

#endif
