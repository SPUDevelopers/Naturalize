#include "Cursor.h"

USING_NS_CC;

Cursor::Cursor() :
	curX(0),
	curY(0),
	tileSize(cocos2d::Size::ZERO)
{
}

Cursor::~Cursor()
{
}

Cursor* Cursor::create(const cocos2d::Size tileSize)
{
	Cursor *ret = new (std::nothrow) Cursor();

	if (ret->init(tileSize))
	{
		// Enable garbage collection
		ret->autorelease();
		return ret;
	}
	else
	{
		// Delete the instance
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool Cursor::init(const cocos2d::Size tileSize)
{
	// Super init
	if (!cocos2d::Node::init())
		return false;
	
	log("*** CURSOR INIT ***");
	
	// Cache map grid size
	this->tileSize = tileSize;

	// Create sprite and attach
	this->cursprite = cocos2d::Sprite::create("maps/test/cursorsprite.png");
	this->cursprite->setPosition(Vec2(64, 64));
	this->addChild(this->cursprite);

	return true;
};

void Cursor::move(int dx, int dy)
{
	curX += dx * (int)(this->tileSize.width);
	curY += dy * (int)(this->tileSize.height);
	this->setPosition(Vec2(curX,curY));
}

void Cursor::moveToXY(int x, int y)
{
	curX = x * (int)(this->tileSize.width);
	curY = y * (int)(this->tileSize.height);
	this->runAction(Repeat::create(MoveTo::create(0.1, Vec2(curX, curY)), 1));
}

int Cursor::getX()
{
	return curX;
}

int Cursor::getY()
{
	return curY;
}
