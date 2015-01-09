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

#pragma mark - GAMESCENE INIT

bool GameScene::init()
{
	// Super init
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// Create a tmx tile map
	std::string mapFilename = "maps/test/test.tmx";

	this->map = TMXTiledMap::create(mapFilename);
	this->map->setScale(1, 1);
	this->addChild(map, 0, -1);

	// Init the tile manager
	this->tileMgr.init(mapFilename, this->map);
	
	//TMXObjectGroup *objectGroup = this->map->objectGroupNamed("objects");
	//ValueMap object = objectGroup->objectNamed("lumberjack");
	//auto description = object[0].;
	//this->addChild(object, 0, 0);
	
	// All tiles are aliased by default: set them anti-aliased
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	
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

#pragma mark - SCENE UPDATE

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

#pragma mark - MOVE MAP

void GameScene::moveMap(char direction)
{
	Vec2 mapPosition = this->map->getPosition();
	Vec2 movePosition = Vec2(0, 0);
	
	Size mapSize = this->map->getMapSize();
	Size windowSize = Director::getInstance()->getVisibleSize();
	
	// Check for being at map bounderies and return direction "n"
	if (mapPosition.y > -128 && direction == 'd') direction = 'n';
	if (mapPosition.x > -128 && direction == 'l') direction = 'n';
	if (mapPosition.y < ((-128 * (mapSize.height - 1)) + windowSize.height) && direction == 'u') direction = 'n';
	if (mapPosition.x < ((-128 * (mapSize.width - 1)) + windowSize.width) && direction == 'r') direction = 'n';
	
	// Set move position
	if (direction == 'u') movePosition = Vec2(0, -128);
	else if (direction == 'd') movePosition = Vec2(0, 128);
	else if (direction == 'l') movePosition = Vec2(128, 0);
	else if (direction == 'r') movePosition = Vec2(-128, 0);
	else log("Map Boundary Reached");

	MoveBy* mapMove = MoveBy::create(0.2, movePosition);
	this->map->runAction(Repeat::create(mapMove, 1));
}

#pragma mark - CONTROL EVENTS

void GameScene::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	log("keyPressed");
	
	if (GAMESTATE_SELECT == cur_state)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			log("UP_ARROW was pressed");
			this->moveMap('u');

			this->cursor.move(0, 1);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			log("DOWN_ARROW was pressed");
			this->moveMap('d');

			this->cursor.move(0, -1);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			log("LEFT_ARROW was pressed");
			this->moveMap('l');

			this->cursor.move(-1, 0);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			log("RIGHT_ARROW was pressed");
			this->moveMap('r');

			this->cursor.move(1, 0);
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
}

void GameScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesMoved");
	
	auto touch = *touches.begin();
	//this->moveMap(touch->getDelta());
}

void GameScene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesEnded");
}

void GameScene::onTouchesCancelled(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event)
{
	log("touchesCancelled");
}