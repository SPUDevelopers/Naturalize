#include "TileMgr.h"

#include "tinyxml2/tinyxml2.h"

TileMgr::TileMgr()
{
}

TileMgr::~TileMgr()
{
}

bool TileMgr::init(const std::string &tmxFile, cocos2d::TMXTiledMap *tmxMap)
{
	if (nullptr == tmxMap) return false; // Invalid pointer

	// Cache Tiled map pointer
	this->tmxMap = tmxMap;
	
	// Get map info
	this->tmxMapInfo = cocos2d::TMXMapInfo::create(tmxFile);
	
	if (!this->tmxMapInfo)
	{
		cleanup();
		return false; // Failed to retrieve map info
	}
	
	// Get tile definition filename
	cocos2d::ValueMap mapProperties = this->tmxMap->getProperties();
	std::string filename = mapProperties["tiledefs"].asString();

	if (!filename.length())
	{
		cleanup();
		return false; // Failed to retrieve tile definition filename
	}

	// Load tileset definition from file
	return loadTileset(filename);
}

void TileMgr::cleanup()
{
	this->tmxMap = nullptr;

	if (this->tmxMapInfo)
	{
		this->tmxMapInfo->release();
		this->tmxMapInfo = nullptr;
	}
}

bool TileMgr::loadTileset(const std::string &filename)
{
	log("Opening tile definitions: %s", filename);

	tinyxml2::XMLError err;

	// Open XML document
	tinyxml2::XMLDocument doc;
	err = doc.LoadFile(filename.c_str());
	
	if (err)
	{
		log("Error opening XML document");
		return false;
	}

	// Clear tile definition list
	tileDefs.clear();

	// Get tileset element
	tinyxml2::XMLElement *tileset = doc.FirstChildElement("tileset");
	
	//
	//	Process all tile definitions in file
	//

	int numOfSuccesses = 0;

	for (tinyxml2::XMLElement *curTile = tileset->FirstChildElement("tile"); curTile; curTile = curTile->NextSiblingElement("tile"))
	{
		std::string tileName = curTile->Attribute("name"); // Get tile name

		// Check if tile exists, if so, skip it!
		TileDefMap::iterator temp_it = tileDefs.find(tileName);

		if (temp_it != tileDefs.end())
		{
			log("Found duplicate tile definition \"%s\". Skipping...", tileName);
			continue;
		}

		// Create new tile definition
		MapTile newTile = MapTile();

		//
		//	Process buffs
		//

		for (tinyxml2::XMLElement *curBuff = curTile->FirstChildElement("buff"); curBuff; curBuff = curBuff->NextSiblingElement("buff"))
		{
			std::string buffType = curBuff->Attribute("type");
			std::string faction = curBuff->Attribute("faction");
			
			// Get buff value
			int value = 0;
			err = curBuff->QueryIntText(&value);

			if (err)
			{
				log("Failed to parse value \"%s\".  Skipping...", curBuff->GetText());
				continue;
			}

			// Parse buff
			if (!setTileBuff(buffType, faction, value, &newTile))
			{
				log("Failed to set %s %s buff = %d.", faction, buffType, value);
				continue;
			}
		}

		//
		//	Process movement costs
		//

		for (tinyxml2::XMLElement *curMoveCost = curTile->FirstChildElement("movecost"); curMoveCost; curMoveCost = curMoveCost->NextSiblingElement("movecost"))
		{
			std::string moveType = curMoveCost->Attribute("type");

			// Get movement cost value
			int value = 0;
			err = curMoveCost->QueryIntText(&value);

			if (err)
			{
				log("Failed to parse value \"%s\".  Skipping...", curMoveCost->GetText());
				continue;
			}

			// Parse movement cost
			if (!setTileMoveCost(moveType, value, &newTile))
			{
				log("Failed to set %s movement cost = %d.", moveType, value);
				continue;
			}
		}

		//
		//	Add tile to definition map
		//

		tileDefs[tileName] = newTile;
		++numOfSuccesses;
	}

	if (tileDefs.empty())
	{
		log("No definitions were loaded.");
		return false;
	}

	log("Successfully loaded %d tiles from %s.", numOfSuccesses, filename);

	return true;
}

bool TileMgr::setTileBuff(const std::string &type, const std::string &faction, const int value, MapTile *mapTile)
{


	return true;
}

bool TileMgr::setTileMoveCost(const std::string &type, const int value, MapTile *mapTile)
{


	return true;
}

MapTile TileMgr::getTileFromXY(cocos2d::Point pt)
{
	return getTileFromXY(pt.x, pt.y);
}

MapTile TileMgr::getTileFromXY(int x, int y)
{
	std::string tilename;
	
	// TODO: Also, to grab a specific tile attribute, get the GID from the tile, then
	//		 match that up with a specific tile to get the tile name, which is used to
	//		 reference internally.

	return getTileFromType(tilename);
}

MapTile TileMgr::getTileFromType(const std::string &tilename)
{
	// Reference the internal tileset list and return a MapTile object

	return MapTile();
}
