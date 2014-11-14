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
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// Create a tmx map
	this->map = TMXTiledMap::create("maps/field/desert.tmx");
	this->map->setScale(2, 2);
	this->addChild(map, 0, -1);
	
	// all tiles are aliased by default, let's set them anti-aliased
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	
	// Schedule update
	scheduleUpdate();

	setKeyboardEnabled(true);

	return true;
}

void GameScene::update(float delta)
{
	// Doing input here is bad! Use event handlers instead...

	float x, y;
	map->getPosition(&x, &y);

	float speed = 100;

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		x += delta * speed;
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		x -= delta * speed;
	}

	if (GetKeyState(VK_UP) & 0x8000)
	{
		y += delta * speed;
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		y -= delta * speed;
	}

	map->setPosition(x, y);
}
