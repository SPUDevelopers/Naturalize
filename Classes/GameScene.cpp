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
	std::string mapFilename = "maps/test/test.tmx";

	this->map = TMXTiledMap::create(mapFilename);
	this->map->setScale(1, 1);
	this->addChild(map, 0, -1);

	//TMXObjectGroup *objectGroup = this->map->objectGroupNamed("objects");
	//ValueMap object = objectGroup->objectNamed("lumberjack");
	//auto description = object[0].;
	//this->addChild(object, 0, 0);

	// All tiles are aliased by default: set them anti-aliased
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}


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
			unsigned short key = ((unitX & 0xFF) << 8) | (unitY & 0xFF);

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
			unsigned short key = ((unitX & 0xFF) << 8) | (unitY & 0xFF);

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
			unsigned short key = ((unitX & 0xFF) << 8) | (unitY & 0xFF);

			UNITLIST::iterator it = this->unitList.find(key);

			// Check unit occupancy list
			if (it != this->unitList.end())
			{
				log("Searching for valid move tiles");
				updateMoveTiles(it->second);
				log("Done searching");

				for (CANMOVETILELIST::iterator cmt_it = moveTileList.begin(); cmt_it != moveTileList.end(); cmt_it++)
				{
					Sprite *mySprite = Sprite::create("maps/test/cursorsprite.png");
					this->map->addChild(mySprite);
					mySprite->setPosition(cmt_it->second.x * this->tileSize, cmt_it->second.y * this->tileSize);
				}
			}
		}
	}
}

void GameScene::keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	log("keyReleased");
}

void GameScene::onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesBegan");
	
	auto touch = *touches.begin();
	
	Vec2 location = touch->getLocation();
	this->cursor->moveToXY((location.x / this->tileSize), (location.y / this->tileSize));
}

void GameScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesMoved");
	auto touch = *touches.begin();
	Vec2 delta = touch->getDelta();
	//this->moveSceneXY(delta.x, delta.y);
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
// Unit stuff

void GameScene::updateMoveTiles(Unit *unit)
{
	// Empty the list of valid movement tiles
	moveTileList.clear();

	// Start the recursion!!!!
	checkMove(unit->getX(), unit->getY(), -1, -1, unit->getMovSpd(), unit->getMovSpd(), unit->getMovType());
}

void GameScene::checkMove(int x, int y, int prevX, int prevY, int curMovePoints, int totalMovePoints, MoveType type)
{
	MapTile tile;

	if (!this->tileMgr.getTileFromXY(&tile, x, y))
	{
		// Tile does not exist, out of bounds!
		return;
	}

	// Calculate movement points left after going to this tile
	if (type <= MOVETYPE_UNKNOWN || type >= MOVETYPE_COUNT) type = MOVETYPE_INFANTRY;

	int pointsLeft = curMovePoints - ((prevX == -1 || prevY == -1) ? 0 : tile.moveCost[type]);
	int totalCost = totalMovePoints - pointsLeft;

	// Check if tile is out of range
	if (pointsLeft < 0)
		return;

	// Make key value
	unsigned short key = ((x & 0xFF) << 8) | (y & 0xFF);

	// Check if the current tile is already in the list of accessible tiles
	CANMOVETILELIST::iterator it = moveTileList.find(key);

	if (it != moveTileList.end())
	{
		if (totalCost >= it->second.costToGetHere)
			return; // If this current path is not optimal, quit!
	}

	// Update 
	moveTileList[key].x = x;
	moveTileList[key].y = y;
	moveTileList[key].prevX = prevX;
	moveTileList[key].prevY = prevY;
	moveTileList[key].costToGetHere = totalCost;

	// If exhausted all points, quit!
	if (pointsLeft == 0)
		return;

	// Recursively check around this tile (with exception of previous tile... unless previous tile is invalid)
	if ((prevX == -1 && prevY == -1) || !(x + 1 == prevX && y == prevY))
		checkMove(x, y + 1, x, y, pointsLeft, totalMovePoints, type);
	if ((prevX == -1 && prevY == -1) || !(x - 1 == prevX && y == prevY))
		checkMove(x, y - 1, x, y, pointsLeft, totalMovePoints, type);
	if ((prevX == -1 && prevY == -1) || !(x == prevX && y - 1 == prevY))
		checkMove(x + 1, y, x, y, pointsLeft, totalMovePoints, type);
	if ((prevX == -1 && prevY == -1) || !(x == prevX && y + 1 == prevY))
		checkMove(x - 1, y, x, y, pointsLeft, totalMovePoints, type);
	
}