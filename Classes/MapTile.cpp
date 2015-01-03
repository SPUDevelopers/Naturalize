//
//  MapTile.cpp
//
//  Created by Evan McNulty on 12/21/14.
//

#include "MapTile.h"

MapTile::MapTile()
{
	// Init buffs
	for (int i = 0; i < Naturalize::FACTION_COUNT; i++)
	{
		this->atkBuf[i] = 0;
		this->defBuf[i] = 0;
	}

	// Init move costs
	for (int i = 0; i < Naturalize::MOVETYPE_COUNT; i++)
	{
		this->moveCost[i] = 0;
	}
}

MapTile::~MapTile()
{
}