#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "TileMgr.h"
#include "Cursor.h"

enum GameSceneState
{
	GAMESTATE_SELECT = 0,	// Inital unit selectio
	GAMESTATE_ACTION,		// Unit selected, action menu
	GAMESTATE_TARGET,		// Select target for action
	GAMESTATE_WAIT			// Waiting for animation/movement
};

class GameScene : public cocos2d::Layer
{
private:
	// Holds current game state
	GameSceneState cur_state;

	// Movement/selection cursor
	Cursor cursor;

//
//	Public class variables
//
public:
	cocos2d::TMXTiledMap* map;
	TileMgr tileMgr;

	float panSpeed = 10;
	
	/////////////////////////////////////////////////////////////////////////
	// Game Loop

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	// init() method is called through the create() method
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// Static method to create instance of scene -- use in conjunction with scene director
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	
	/////////////////////////////////////////////////////////////////////////
	// Game Loop

	// Update method
	void update(float delta);

	// State-specific routines (to separate implementations)
	void updateSelectState(float delta);
	void updateActionState(float delta);
	void updateTargetState(float delta);
	void updateWaitState(float delta);

	/////////////////////////////////////////////////////////////////////////
	// Input
	
	// Keyboard event handlers
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	
	// Touch screen event handlers
	void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);

	/////////////////////////////////////////////////////////////////////////
	// Map

	// Map movement method
	void moveMap(char direction);

	/////////////////////////////////////////////////////////////////////////
	// Callbacks

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif // __GAME_SCENE_H__
