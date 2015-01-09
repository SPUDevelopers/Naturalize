#include "TileMgr.h"

#include "NatTypes.h"
#include "StringHelpers.h"
#include "tinyxml2/tinyxml2.h"

TileMgr::TileMgr() :
	tmxMap(nullptr),
	tmxMapInfo(nullptr)
{
}

TileMgr::~TileMgr()
{
	this->tileDefs.clear();
	cleanup();
}

bool TileMgr::init(const std::string &tmxFile, cocos2d::TMXTiledMap *tmxMap)
{
	if (nullptr == tmxMap)
	{
		return false; // Invalid pointer
	}

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
	log("Opening tile definitions: %s", filename.c_str());

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
	this->tileDefs.clear();

	// Get tileset element
	tinyxml2::XMLElement *tileset = doc.FirstChildElement("tileset");
	
	//
	//	Process all tile definitions in file
	//

	int numOfSuccesses = 0;

	for (tinyxml2::XMLElement *curTile = tileset->FirstChildElement("tile"); curTile; curTile = curTile->NextSiblingElement("tile"))
	{
		std::string tileName = curTile->Attribute("name"); // Get tile name
		tileName = StringToLower(tileName); // Convert to lowercase

		// Check if tile exists, if so, skip it!
		TileDefinitions::iterator temp_it = this->tileDefs.find(tileName);

		if (temp_it != this->tileDefs.end())
		{
			log("Found duplicate tile definition \"%s\". Skipping...", tileName.c_str());
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
				log("Failed to set %s %s buff = %d.", faction.c_str(), buffType.c_str(), value);
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
				log("Failed to set %s movement cost = %d.", moveType.c_str(), value);
				continue;
			}
		}

		//
		//	Add tile to definition map
		//

		this->tileDefs[tileName] = newTile;
		++numOfSuccesses;
	}

	if (this->tileDefs.empty())
	{
		log("No definitions were loaded.");
		return false;
	}

	log("Successfully loaded %d tiles from %s.", numOfSuccesses, filename.c_str());

	return true;
}

bool TileMgr::setTileBuff(const std::string &typeName, const std::string &factionName, const int value, MapTile *mapTile)
{
	//
	//	Determine faction
	//

	int faction = Naturalize::GetFactionFromString(factionName);

	if (faction <= Naturalize::FACTION_UNKNOWN || faction >= Naturalize::FACTION_COUNT)
	{
		log("Unknown faction type \"%s\" found!", factionName.c_str());
		return false;
	}

	//
	//	Determine buff type
	//

	std::string lowerType = StringToLower(typeName);

	if (lowerType == "attack")
	{
		// Apply attack buff
		mapTile->atkBuf[faction] = value;
	}
	else if (lowerType == "defense")
	{
		// Apply defensive buff
		mapTile->defBuf[faction] = value;
	}
	else
	{
		log("Unknown buff type \"%s\" found!", typeName.c_str());
		return false;
	}

	return true;
}

bool TileMgr::setTileMoveCost(const std::string &typeName, const int value, MapTile *mapTile)
{
	//
	//	Determine movement type
	//

	int movetype = Naturalize::GetMoveTypeFromString(typeName);

	if (movetype <= Naturalize::MOVETYPE_UNKNOWN || movetype >= Naturalize::MOVETYPE_COUNT)
	{
		log("Unknown movement type \"%s\" found!", typeName.c_str());
		return false;
	}

	// Apply movement cost
	mapTile->moveCost[movetype] = value;

	return true;
}

MapTile TileMgr::getTileFromXY(cocos2d::Point pt)
{
	if (nullptr == this->tmxMap)
	{
		log("Missing TMX pointer!");
		return MapTile(); // Return default tile
	}

	//
	//	Retrieve GID of tile
	//
	
	// Only look at tiles from background layer (Layer 0)
	int gid = this->tmxMap->getLayer("background")->getTileGIDAt(pt);

	cocos2d::ValueMap properties = this->tmxMap->getPropertiesForGID(gid).asValueMap();

	if (!properties.size())
	{
		log("Failed to obtain tile properties!");
		return MapTile(); // Return default tile
	}

	//
	//	Get name of tile
	//

	std::string tilename = properties["name"].asString();

	if (!tilename.length())
	{
		log("Failed to obtain tile name!");
		return MapTile();
	}

	return getTileFromType(tilename); // Get tile from type name
}

MapTile TileMgr::getTileFromXY(int x, int y)
{
	return getTileFromXY(cocos2d::Point(x, y));
}

MapTile TileMgr::getTileFromType(const std::string &tilename)
{
	std::string lowerName = StringToLower(tilename); // Convert to lowercase

	// Find tile in definition set
	TileDefinitions::iterator it = this->tileDefs.find(lowerName);

	if (it == this->tileDefs.end())
	{
		log("Could not find tile definition for tile: %s", tilename.c_str());
		return MapTile(); // Return default tile
	}

	return it->second;
}

int TileMgr::getCount()
{
	return this->tileDefs.size();
}
