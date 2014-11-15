#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	
	// Public class variables
	cocos2d::TMXTiledMap* map;
	float panSpeed = 10;
	
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// Update method
	void update(cocos2d::Vec2 position);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	
	// Keyboard event handlers
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	
	// Touch screen event handlers
	void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);

};

#endif // __GAME_SCENE_H__
