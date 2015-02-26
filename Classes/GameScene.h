#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#include "TileMgr.h"
#include "Cursor.h"
#include "Unit.h"

#include <map>

enum GameSceneState
{
	GAMESTATE_SELECT = 0,	// Inital unit selectio
	GAMESTATE_ACTION,		// Unit selected, action menu
	GAMESTATE_TARGET,		// Select target for action
	GAMESTATE_WAIT			// Waiting for animation/movement
};

// Enum of directions. These can be used separately, or OR-ed together for combinations
enum Direction
{
	DIRECTION_LEFT	= 0x1,
	DIRECTION_UP	= 0x2,
	DIRECTION_RIGHT	= 0x4,
	DIRECTION_DOWN	= 0x8
};

class GameScene : public cocos2d::Layer
{
	///////////////////////////////////////////////////
	// TESTING

private:
	Label *debugText;
	std::list<Sprite *> moveSpriteList;

	//
	///////////////////////////////////////////////////

private:
	// Holds current game state
	GameSceneState cur_state;

	// Movement/selection cursor
	Cursor *cursor;
	
	// Map position relative to screen (in px)
	cocos2d::Vec2 mapPixelPosition;

	// Tile size
	float tileSize;
	
//
//	Public class variables
//
public:
	cocos2d::TMXTiledMap* map;
	TileMgr tileMgr;

	float panSpeed = 10;
	
//
//	Module-specific code
//

	/////////////////////////////////////////////////////////////////////////
	// Units

private:
	typedef std::map<unsigned short, Unit*> UNITLIST;
	UNITLIST unitList; // List of all units in play

	// Unit key helpers
	unsigned short packUnitKey(int x, int y);
	void unpackUnitKey(unsigned short key, int *px, int *py);

	// Movement tile class
	class CanMoveTile
	{
	public:
		int prevX, prevY;	// Previous tile where came from
		int costToGetHere;	// Cost to get to this tile
	};
	
	// List of valid movement tiles
	typedef std::map<unsigned short, CanMoveTile> CANMOVETILELIST;
	CANMOVETILELIST moveTileList;

	// Recalculates all possible movement tiles for a given unit.
	void updateMoveTiles(Unit *unit);

	// THIS FUNCTION SHOULD NOT BE CALLED OUTSIDE OF updateMoveTiles!!!
	// Recursively checks all possible movement tiles based on cost.
	void checkMove(int x, int y, int prevX, int prevY, int curCost, int maxCost, MoveType type, bool atStart = false);

//
//	Methods
//
public:
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
	// Unit Movement



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

	// Map movement method (dir is the direction the cursor moved in)
	void updateMapMovement(Direction dir);

	// Moves the map to the specified x,y (in px)
	void moveScene(Vec2 position);
	void moveSceneX(float positionX);
	void moveSceneY(float positionY);

	/////////////////////////////////////////////////////////////////////////
	// Callbacks

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif // __GAME_SCENE_H__
