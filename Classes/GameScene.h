#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	static cocos2d::TMXTiledMap map;
	
	void prepareLayers();
	void createFixtures(cocos2d::TMXLayer* layer);
	void createRectangularFixture(cocos2d::TMXLayer* layer, int x, int y, float width, float height);
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	
};

#endif // __GAME_SCENE_H__
