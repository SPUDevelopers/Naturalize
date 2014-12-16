#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include <map>

#include "MapTile.h"

// Class to assist in loading and managing tile types on the map.
// Each type of tile on the map should have a type ID; this is used to retrieve tile objects.
class TileMgr
{
private:


public:
	TileMgr();
	~TileMgr();

	// Get tile object from specified type
	MapTile getTile(char *typeString);
};

#endif
