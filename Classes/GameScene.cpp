#include "GameScene.h"

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
	
	// Create background sprite
	bgSprite = Sprite::create("menuBackground.png");
	bgSprite->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height /2) + origin.y));
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite);
	
	// Create hero sprite
	heroSprite = Sprite::create("potatoHero.png");
	heroSprite->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height /2) + origin.y));
	this->addChild(heroSprite);
	
	return true;
}
