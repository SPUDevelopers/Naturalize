#include "Cursor.h"

USING_NS_CC;

Cursor::Cursor() :
	curX(0),
	curY(0),
	tileSize(cocos2d::Size::ZERO),
	mapSize(cocos2d::Size::ZERO)
{
}

Cursor::~Cursor()
{
}

Cursor* Cursor::create(const cocos2d::Size tileSize, const cocos2d::Size mapSize)
{
	Cursor *ret = new (std::nothrow) Cursor();

	if (ret->init(tileSize, mapSize))
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

bool Cursor::init(const cocos2d::Size tileSize, const cocos2d::Size mapSize)
{
	// Super init
	if (!cocos2d::Node::init())
		return false;
	
	log("*** CURSOR INIT ***");
	
	// Cache map grid size
	this->tileSize = tileSize;

	// Cache map size (in tiles)
	this->mapSize = mapSize;

	// Create sprite and attach
	this->cursprite = cocos2d::Sprite::create("maps/test/cursorsprite.png");
	this->cursprite->setPosition(Vec2(64, 64));
	this->addChild(this->cursprite);

	return true;
};

void Cursor::move(int dx, int dy)
{
	curX += dx;
	curY += dy;

	// Fix if outside boundaries
	if (curX < 0) curX = 0;
	else if (curX >= this->mapSize.width) curX = this->mapSize.width - 1;

	if (curY < 0) curY = 0;
	else if (curY >= this->mapSize.height) curY = this->mapSize.height - 1;
	
	this->setPosition(Vec2(curX  * this->tileSize.width, curY * this->tileSize.height));
}

void Cursor::move(Vec2 delta)
{
	this->move((int)delta.x, (int)delta.y);
}

void Cursor::moveToXY(int x, int y)
{
	curX = x;
	curY = y;

	// Fix if outside boundaries
	if (curX < 0) curX = 0;
	else if (curX >= this->mapSize.width) curX = this->mapSize.width - 1;

	if (curY < 0) curY = 0;
	else if (curY >= this->mapSize.height) curY = this->mapSize.height - 1;

	this->runAction(Repeat::create(MoveTo::create(0.1, Vec2(curX, curY)), 1));
}

void Cursor::moveToXY(Vec2 delta)
{
	this->moveToXY((int)delta.x, (int)delta.y);
}

int Cursor::getX()
{
	return curX;
}

int Cursor::getY()
{
	return curY;
}

Vec2 Cursor::getXY()
{
	return Vec2(curX, curY);
}