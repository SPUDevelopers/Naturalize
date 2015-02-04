#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "cocos2d.h"

class Cursor : public cocos2d::Node
{
private:
	cocos2d::Sprite *cursprite;

	int curX;
	int curY;

	cocos2d::Size tileSize; // Map tile size (in px)
	cocos2d::Size mapSize; // Map size (in tiles)

public:
	//
	//	Construction
	//

	Cursor();
	~Cursor();

	// Creates a Cursor object with the given grid square size (in px)
	static Cursor* create(const cocos2d::Size tileSize, const cocos2d::Size mapSize);

	// Initializes a Cursor with the specified map grid size
	virtual bool init(const cocos2d::Size tileSize, const cocos2d::Size mapSize);

	//
	//	Methods
	//

	// Moves the cursor relative to its current position.
	// Coordinates are in map tiles.
	void move(int dx, int dy);
	void move(cocos2d::Vec2 delta);

	// Moves the cursor to an absolute position on the map.
	// Coordinates are in map tiles.
	void moveToXY(int x, int y);
	void moveToXY(cocos2d::Vec2 delta);

	int getX();
	int getY();

	cocos2d::Vec2 getXY();

	// Gets the "pixel position" of the cursor, relative to the map
	cocos2d::Vec2 getPixelPosition();
	int getPixelPositionX();
	int getPixelPositionY();
};

#endif
