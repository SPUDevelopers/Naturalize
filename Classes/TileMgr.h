#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include "cocos2d.h"

#include <string>
#include <map>

#include "MapTile.h"

// Class to assist in loading and managing tile types on the map.
// Each type of tile on the map should have a type ID; this is used to retrieve tile objects.
class TileMgr
{
//
//	Member Variables
//
private:
	cocos2d::TMXTiledMap *tmxMap;
	cocos2d::TMXMapInfo *tmxMapInfo;

	typedef std::map<std::string, MapTile> TileDefinitions;
	TileDefinitions tileDefs;

//
//	Construction
//
public:
	TileMgr();
	~TileMgr();

	// Initializes the tile manager. This MUST be called after the map is loaded.
	bool init(const std::string &tmxFile, cocos2d::TMXTiledMap *tmxMap);

	// Cleanup object pointers.
	void cleanup();

//
//	Methods
//
private:
	// Load tile objects from file.
	bool loadTileset(const std::string &filename);

	// Set buff based on buff type and faction.
	bool setTileBuff(const std::string &typeName, const std::string &factionName, const int value, MapTile *mapTile);

	// Set movement cost based on movement type.
	bool setTileMoveCost(const std::string &typeName, const int value, MapTile *mapTile);

public:
	// Get tile object from specified location on map.
	MapTile getTileFromXY(cocos2d::Point pt);

	// Get tile object from specified location on map.
	MapTile getTileFromXY(int x, int y);

	// Get tile object from specified type.
	MapTile getTileFromType(const std::string &tilename);

	// Returns number of tile definitions.
	int getCount();

};

#endif
