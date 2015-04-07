#include "GameScene.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance

bool GameScene::init()
{
	// Super init
	if ( !Layer::init() )
	{
		return false;
	}

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// Create a tmx tile map
	std::string mapFilename = "maps/test/test_sm.tmx";

	this->map = TMXTiledMap::create(mapFilename);
	this->map->setScale(1, 1);
	this->addChild(map, 0, -1);

	//TMXObjectGroup *objectGroup = this->map->objectGroupNamed("objects");
	//ValueMap object = objectGroup->objectNamed("lumberjack");
	//auto description = object[0].;
	//this->addChild(object, 0, 0);

	// All tiles are aliased by default: set them anti-aliased
	/* // Anti-aliasing turned off
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	//*/


	this->tileSize = this->map->getTileSize().width;
	
	// Init the tile manager
	this->tileMgr.init(mapFilename, this->map);
	
	// Cursor init
	this->cursor = Cursor::create(this->map->getTileSize(), this->map->getMapSize());
	this->map->addChild(this->cursor, 0, 0);
	this->mapPixelPosition = Vec2(0, 0);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	// Set up keyboard listener
	auto keyboardListener = EventListenerKeyboard::create();

	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::keyReleased, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
#endif
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Set up touchscreen listener
	auto touchscreenListener = EventListenerTouchAllAtOnce::create();

	touchscreenListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	touchscreenListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	touchscreenListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
	touchscreenListener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchscreenListener, this);
#endif

	// Initialize game state
	cur_state = GAMESTATE_SELECT;


	//////////////////////////////////////////////////////////
	// DEBUG

	debugText = Label::create("<Debug>", "Courier New", 12);
	this->addChild(debugText, 1);
	debugText->setPosition(78, 655);

	// Kick the text update
	keyPressed(EventKeyboard::KeyCode::KEY_0, nullptr);

	//
	//////////////////////////////////////////////////////////


	this->scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	// Log frame data
	//log("Delta: %f FPS: %f", delta, 1 / delta);

	// Call state-specific update methods
	switch (cur_state)
	{
	default:
		log("Bad state: %d", cur_state);
		cur_state = GAMESTATE_SELECT;
	case GAMESTATE_SELECT:
		updateSelectState(delta);
		break;
	case GAMESTATE_ACTION:
		updateActionState(delta);
		break;
	case GAMESTATE_WAIT:
		updateWaitState(delta);
		break;
	}
}

void GameScene::updateSelectState(float delta)
{
	
}

void GameScene::updateActionState(float delta)
{

}

void GameScene::updateTargetState(float delta)
{

}

void GameScene::updateWaitState(float delta)
{

}

void GameScene::updateMapMovement(Direction dir)
{
	// Get cursor position relative to screen (in px)
	int cursorX = (int)(this->cursor->getPixelPositionX() + this->mapPixelPosition.x);
	int cursorY = (int)(this->cursor->getPixelPositionY() + this->mapPixelPosition.y);

	// Get window size (in px)
	Size windowSize = Director::getInstance()->getVisibleSize();

	// Get map width (in px)
	int mapWidth = (int)(this->map->getMapSize().width * this->tileSize);
	int mapHeight = (int)(this->map->getMapSize().height * this->tileSize);
	
	//
	//	Check if cursor is at edge of screen
	//

	if (dir & DIRECTION_RIGHT)
	{
		// Check cursor-screen bounds
		if (cursorX >= (windowSize.width - this->tileSize)) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->mapPixelPosition.x > (windowSize.width - mapWidth))
			{
				// Move map left
				this->moveSceneX(this->mapPixelPosition.x - this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_LEFT)
	{
		// Check cursor-screen bounds
		if (cursorX < this->tileSize) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->mapPixelPosition.x < 0)
			{
				// Move map right
				this->moveSceneX(this->mapPixelPosition.x + this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_UP)
	{
		// Check cursor-screen bounds
		if (cursorY >= (windowSize.height - this->tileSize)) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->mapPixelPosition.y > (windowSize.height - mapHeight))
			{
				// Move map down
				this->moveSceneY(this->mapPixelPosition.y - this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_DOWN)
	{
		// Check cursor-screen bounds
		if (cursorY < this->tileSize) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->mapPixelPosition.y < 0)
			{
				// Move map up
				this->moveSceneY(this->mapPixelPosition.y + this->tileSize);
			}
		}
	}
}

/*void GameScene::move(Vec2 position)
{
	// Set up vars
	Vec2 newPosition = Vec2(floor(position.x / this->tileSize), floor(position.y / this->tileSize)); // Find new cursor position
	Vec2 mapPosition = this->map->getPosition();
	Size mapSize = this->map->getMapSize();
	Size windowSize = Director::getInstance()->getVisibleSize();
	
	// Check for edge of map
	if (newPosition.y >= ((windowSize.height / this->tileSize)))
	{
		this->moveScene(Vec2(mapPosition.x, mapPosition.y - this->tileSize));
		//newPosition.y = (this->cursor->getY() / this->tileSize);
	}
	
	// Move cursor
	this->cursor->moveToXY(newPosition.x, newPosition.y);
}*/

void GameScene::moveSceneTo(Vec2 position) // In pixel coords
{
	// Cache pixel position of map
	this->mapPixelPosition = position;

	//this->map->setPosition(this->mapPixelPosition);
	this->map->stopAllActions();
	this->map->runAction(MoveTo::create(0.4f, this->mapPixelPosition));
}

void GameScene::moveSceneXY(float x, float y) // In pixel coords
{
	Vec2 moveDelta(x, y);
	this->mapPixelPosition = Vec2(this->mapPixelPosition.x + x, this->mapPixelPosition.y + y);
	this->map->stopAllActions();
	this->map->runAction(MoveBy::create(0.4f, moveDelta));
}

void GameScene::moveSceneX(float positionX)
{
	this->moveSceneTo(Vec2(positionX, this->mapPixelPosition.y));
}

void GameScene::moveSceneY(float positionY)
{
	this->moveSceneTo(Vec2(this->mapPixelPosition.x, positionY));
}

void GameScene::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	log("keyPressed");
	
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
		return;
	}

	if (GAMESTATE_SELECT == cur_state)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			log("UP_ARROW was pressed");
			this->cursor->move(0, 1);
			this->updateMapMovement(DIRECTION_UP);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			log("DOWN_ARROW was pressed");
			this->cursor->move(0, -1);
			this->updateMapMovement(DIRECTION_DOWN);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			log("LEFT_ARROW was pressed");
			this->cursor->move(-1, 0);
			this->updateMapMovement(DIRECTION_LEFT);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			log("RIGHT_ARROW was pressed");
			this->cursor->move(1, 0);
			this->updateMapMovement(DIRECTION_RIGHT);
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_DELETE)
		{
			log("DELETE was pressed");

			// Get cursor position
			int unitX = this->cursor->getX();
			int unitY = this->cursor->getY();

			// Make key value
			unsigned short key = packUnitKey(unitX, unitY);
			
			UNITLIST::iterator it = this->unitList.find(key);

			// Check unit occupancy list
			if (it != this->unitList.end())
			{
				log("We found a unit!");
				log("Kill 'em");

				this->map->removeChild(it->second);
				it->second->release(); // Decrement reference count

				this->unitList.erase(it);
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_A)
		{
			log("A was pressed");

			// Get cursor position
			int unitX = this->cursor->getX();
			int unitY = this->cursor->getY();

			// Make key value
			unsigned short key = packUnitKey(unitX, unitY);

			UNITLIST::iterator it = this->unitList.find(key);

			if (it == this->unitList.end())
			{
				Unit *myunit = Unit::create(
					cocos2d::Size(this->tileSize, this->tileSize),
					unitX, unitY, 0);

				this->map->addChild(myunit);
				myunit->setPosition(this->cursor->getPixelPosition());

				// Add unit to master list
				this->unitList[key] = myunit;
				myunit->retain(); // Increment reference count
			}
		}

		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			log("SPACE was pressed");

			// Get cursor position
			int unitX = this->cursor->getX();
			int unitY = this->cursor->getY();

			// Make key value
			unsigned short key = packUnitKey(unitX, unitY);

			UNITLIST::iterator it = this->unitList.find(key);

			// Check unit occupancy list
			if (it != this->unitList.end())
			{
				log("Searching for valid move tiles");
				updateMoveTiles(it->second);
				log("Done searching");

				for (CANMOVETILELIST::iterator cmt_it = moveTileList.begin(); cmt_it != moveTileList.end(); cmt_it++)
				{
					int x, y;
					unpackUnitKey(cmt_it->first, &x, &y);
					
					Sprite *mySprite = Sprite::create("maps/test/validmove.png");
					this->map->addChild(mySprite);
					
					////////////// DEBUG //////////////
					moveSpriteList.push_back(mySprite);
					///////////////////////////////////

					mySprite->setPosition(64 + x * this->tileSize, 64 + y * this->tileSize);
				}
			}
		}

		/////////////////////////////////////////////////////////////////
		// Debug text

		String debugString;

		// Get map position
		int mapX = (int)round(this->mapPixelPosition.x / this->tileSize);
		int mapY = (int)round(this->mapPixelPosition.y / this->tileSize);

		debugString.appendWithFormat("   Map: (%2d, %2d)\n", mapX, mapY);

		// Get cursor position
		int curX = this->cursor->getX();
		int curY = this->cursor->getY();

		debugString.appendWithFormat("Cursor: (%2d, %2d)\n", curX, curY);

		// Check for unit
		unsigned short key = packUnitKey(curX, curY);
		UNITLIST::iterator it = this->unitList.find(key);

		if (it != unitList.end())
		{
			debugString.append("<UNIT FOUND>\n");
		}
		else
		{
			debugString.append("------\n");
		}

		debugString.append("\n");

		// Check maptile
		int tiledX = this->cursor->getX();
		int tiledY = (this->map->getMapSize().height - 1) - this->cursor->getY();

		debugString.appendWithFormat("TilePt: (%2d, %2d)\n", tiledX, tiledY);
		debugString.append("\n");

		int gid = this->map->getLayer("background")->getTileGIDAt(Point(tiledX, tiledY));

		debugString.appendWithFormat(" Tile GID: %d\n", gid);

		if (gid)
		{
			cocos2d::ValueMap properties = this->map->getPropertiesForGID(gid).asValueMap();
			std::string tilename = properties["name"].asString();

			debugString.appendWithFormat("Tile Name: %-6s\n", tilename.c_str());
		}
		
		// Get tile cost
		MapTile tile;
		this->tileMgr.getTileFromXY(&tile, this->cursor->getXY());
		
		debugString.appendWithFormat("Tile Cost: %d\n", tile.moveCost[MOVETYPE_INFANTRY]);

		debugString.append("\n");
		
		// Check for valid move tiles
		CANMOVETILELIST::iterator moveIt = moveTileList.find(key);

		if (moveIt != moveTileList.end())
		{
			debugString.append("<MOVETILE FOUND>\n");
			debugString.appendWithFormat("PrevXY: (%2d, %2d)\n", moveIt->second.prevX, moveIt->second.prevY);
			debugString.appendWithFormat("Total Cost: %d\n", moveIt->second.costToGetHere);
		}
		else
		{
			debugString.append("------\n\n\n");
		}

		// Set the text of the label
		debugText->setString(debugString.getCString());

		//
		/////////////////////////////////////////////////////////////////
	}
}

void GameScene::keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	log("keyReleased");
}

void GameScene::onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	auto touch = *touches.begin();
	Vec2 location = touch->getLocation();
	log("touchesBegan: X:%f Y:%f", location.x, location.y);
	this->cursor->moveToXY(((location.x - mapPixelPosition.x) / this->tileSize), ((location.y - mapPixelPosition.y) / this->tileSize));
}

void GameScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	auto touch = *touches.begin();
	Vec2 location = touch->getDelta();
	log("touchesMoved: X:%f Y:%f", location.x, location.y);
	Vec2 moveTo = Vec2(mapPixelPosition.x + location.x, mapPixelPosition.y + location.y);

	// Check boundaries
	Size windowSize = Director::getInstance()->getVisibleSize();
	if (moveTo.x >= ((this->map->getMapSize().width * this->tileSize - windowSize.width) * -1) && moveTo.x <= 0 )
	{
		this->mapPixelPosition = Vec2(moveTo.x, mapPixelPosition.y);
		this->map->setPosition(mapPixelPosition);
	}
	
	if (moveTo.y >= ((this->map->getMapSize().height * this->tileSize - windowSize.height) * -1) && moveTo.y <= 0 )
	{
		this->mapPixelPosition = Vec2(mapPixelPosition.x, moveTo.y);
		this->map->setPosition(mapPixelPosition);
	}
	
}

void GameScene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesEnded");
}

void GameScene::onTouchesCancelled(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesCancelled");
}


///////////////////////////////////////////////////////////////////////////////////////
// Unit methods

//
//	Unit helpers
//

unsigned short GameScene::packUnitKey(int x, int y)
{
	return (unsigned short)((x & 0xFF) << 8) | (y & 0xFF);
}

void GameScene::unpackUnitKey(unsigned short key, int *px, int *py)
{
	*px = (key >> 8) & 0xFF;
	*py = key & 0xFF;
}

//
//	Unit movement
//

void GameScene::updateMoveTiles(Unit *unit)
{
	///////// DEBUG ////////////
	// Clear all valid move sprites from the screen
	for (std::list<Sprite *>::iterator it = moveSpriteList.begin(); it != moveSpriteList.end(); it++)
	{
		this->map->removeChild((*it));
	}
	////////////////////////////

	// Empty the list of valid movement tiles
	moveTileList.clear();

	// Start the recursion!!!!
	checkMove(unit->getX(), unit->getY(), -1, -1, 0, unit->getMovSpd(), unit->getMovType(), true);
}

void GameScene::checkMove(int x, int y, int prevX, int prevY, int curCost, int maxCost, MoveType type, bool atStart)
{
	MapTile tile;

	if (!this->tileMgr.getTileFromXY(&tile, x, y))
	{
		// Tile does not exist, out of bounds!
		return;
	}
	
	if (type <= MOVETYPE_UNKNOWN || type >= MOVETYPE_COUNT)
	{
		log("Unknown movement type encountered!");
		type = MOVETYPE_INFANTRY;
	}

	// Calculate cost of going to this tile
	int totalCost = 0;

	// Only add the cost of the current tile if we have moved from the start
	if (!atStart)
	{
		totalCost = curCost + tile.moveCost[type];
	}

	// Check if tile is out of range
	if (totalCost > maxCost)
	{
		return;
	}

	// Check if the current tile is already in the list of accessible tiles
	unsigned short key = packUnitKey(x, y);
	CANMOVETILELIST::iterator it = moveTileList.find(key);

	if (it != moveTileList.end())
	{
		if (totalCost >= it->second.costToGetHere)
		{
			return; // If this current path is not optimal, quit!
		}
	}

	// Update 
	moveTileList[key].prevX = prevX;
	moveTileList[key].prevY = prevY;
	moveTileList[key].costToGetHere = totalCost;

	// If exhausted all points, quit!
	if (totalCost >= maxCost)
	{
		return;
	}

	//
	//	Recursively check around this tile
	//

	if (atStart || !(x == prevX && y + 1 == prevY)) // Check up
		checkMove(x, y + 1, x, y, totalCost, maxCost, type);
	
	if (atStart || !(x + 1 == prevX && y == prevY)) // Check right
		checkMove(x + 1, y, x, y, totalCost, maxCost, type);

	if (atStart || !(x == prevX && y - 1 == prevY)) // Check down
		checkMove(x, y - 1, x, y, totalCost, maxCost, type);

	if (atStart || !(x - 1 == prevX && y == prevY)) // Check left
		checkMove(x - 1, y, x, y, totalCost, maxCost, type);
}

GameScene::CanMoveTile* GameScene::returnTileStruct(int x, int y)
{
	unsigned short key = packUnitKey(x, y);
	CANMOVETILELIST::iterator it = moveTileList.find(key);

	if (it != moveTileList.end())
	{
		return(&it->second);
	}
	return nullptr;
}

void GameScene::fillMoveStack(int x, int y, std::stack<unsigned short> &stack)
{
	CanMoveTile* tile;

	int xCord = x;
	int yCord = y;

	stack.push(packUnitKey(xCord, yCord));

	while (tile = returnTileStruct(xCord, yCord), tile != nullptr)
	{
		xCord = tile->prevX;
		yCord = tile->prevY;
		stack.push(packUnitKey(xCord, yCord));
	}
}