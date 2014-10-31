#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//mySprite = Sprite::create("newgame.png");
	//mySprite->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height /2) + origin.y));
	//this->addChild(mySprite);
	
	auto startGameButtonImage = MenuItemImage::create("newGame.png", "newGame.png", CC_CALLBACK_1(MainMenuScene::startGameCallback, this));
	auto menuCloseButtonImage = MenuItemImage::create("exitGame.png", "exitGame.png", CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
	
	startGameButtonImage->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height) - (visibleSize.height/3)));
	menuCloseButtonImage->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height /3) + origin.y));
	
	auto startGameButtonMenu = Menu::create(startGameButtonImage, NULL);
	startGameButtonMenu->setPosition(Point::ZERO);
	this->addChild(startGameButtonMenu, 1);
	
	auto menuCloseButtonMenu = Menu::create(menuCloseButtonImage, NULL);
	menuCloseButtonMenu->setPosition(Point::ZERO);
	this->addChild(menuCloseButtonMenu, 1);
	
    return true;
}

void MainMenuScene::startGameCallback(Ref* pSender)
{
	Director::getInstance()->pushScene(GameScene::createScene());
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
