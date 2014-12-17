#ifndef __CURSOR_H__
#define __CURSOR_H__
#include "cocos2d.h"

class Cursor : public cocos2d::Node
{
private:
	cocos2d::Sprite *cursprite;
	int curX;
	int curY;

public:
	Cursor();
	~Cursor();

	CREATE_FUNC(Cursor);

	virtual bool init()
	{
		if (!cocos2d::Node::init())
			return false;

		this->cursprite = cocos2d::Sprite::create("cursorsprite.png");
		this->addChild(this->cursprite);
		return true;
	};


	// Moves the cursor relative to its current position.
	void move(int dx, int dy);

	// Moves the cursor to an absolute position on the map.
	void moveToXY(int x, int y);

	int getX();

	int getY();
};

#endif
