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

public:
	//
	//	Construction
	//

	Cursor();
	~Cursor();

	// Creates a Cursor object with the given grid square size (in px)
	static Cursor* create(const cocos2d::Size tileSize);

	// Initializes a Cursor with the specified map grid size
	virtual bool init(const cocos2d::Size tileSize);

	//
	//	Methods
	//

	// Moves the cursor relative to its current position.
	// Coordinates are in map tiles.
	void move(int dx, int dy);

	// Moves the cursor to an absolute position on the map.
	// Coordinates are in map tiles.
	void moveToXY(int x, int y);

	int getX();

	int getY();
};

#endif
