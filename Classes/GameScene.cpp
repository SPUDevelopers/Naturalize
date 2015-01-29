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
	int cursorX = (int)(this->cursor->getPositionX() + this->map->getPositionX());
	int cursorY = (int)(this->cursor->getPositionY() + this->map->getPositionY());

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
			if (this->map->getPositionX() > (windowSize.width - mapWidth))
			{
				// Move map left
				this->map->setPositionX(this->map->getPositionX() - this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_LEFT)
	{
		// Check cursor-screen bounds
		if (cursorX < this->tileSize) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->map->getPositionX() < 0)
			{
				// Move map right
				this->map->setPositionX(this->map->getPositionX() + this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_UP)
	{
		// Check cursor-screen bounds
		if (cursorY >= (windowSize.height - this->tileSize)) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->map->getPositionY() > (windowSize.height - mapHeight))
			{
				// Move map down
				this->map->setPositionY(this->map->getPositionY() - this->tileSize);
			}
		}
	}
	
	if (dir & DIRECTION_DOWN)
	{
		// Check cursor-screen bounds
		if (cursorY < this->tileSize) // Cursor is within 1 tile from edge
		{
			// Check map-screen bounds
			if (this->map->getPositionY() < 0)
			{
				// Move map up
				this->map->setPositionY(this->map->getPositionY() + this->tileSize);
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

void GameScene::moveScene(Vec2 position) // In pixel coords
{
	this->runAction(Repeat::create(MoveTo::create(0.2, Vec2(position.x , position.y)), 1));
	//if (direction == 'u') this->map->runAction(Repeat::create(MoveBy::create(0.2, Vec2(0, (this->tileSize * -1))), 1));
	//else if (direction == 'd') this->map->runAction(Repeat::create(MoveBy::create(0.2, Vec2(0, this->tileSize)), 1));
	//else if (direction == 'l') this->map->runAction(Repeat::create(MoveBy::create(0.2, Vec2(this->tileSize, 0)), 1));
	//else if (direction == 'r') this->map->runAction(Repeat::create(MoveBy::create(0.2, Vec2((this->tileSize * -1), 0)), 1));
}

void GameScene::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	log("keyPressed");
	
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
	
	// TODO: Convert this to tile coordinates
	this->cursor->moveToXY(touch->getLocation());
}

void GameScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesMoved");
	auto touch = *touches.begin();
	this->moveScene(touch->getDelta());
}

void GameScene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesEnded");
}

void GameScene::onTouchesCancelled(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesCancelled");
}