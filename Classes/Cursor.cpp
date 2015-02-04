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
	int prevX = this->curX;
	int prevY = this->curY;

	this->curX += dx;
	this->curY += dy;

	// Fix if outside boundaries
	if (this->curX < 0) this->curX = 0;
	else if (this->curX >= this->mapSize.width) this->curX = this->mapSize.width - 1;

	if (this->curY < 0) this->curY = 0;
	else if (this->curY >= this->mapSize.height) this->curY = this->mapSize.height - 1;
	
	// Calc new position deltas
	Vec2 position = Vec2((this->curX - prevX) * this->tileSize.width, (this->curY - prevY) * this->tileSize.height);
	this->runAction(MoveBy::create(0.2f, position)); // Run action
}

void Cursor::move(Vec2 delta)
{
	this->move((int)delta.x, (int)delta.y);
}

void Cursor::moveToXY(int x, int y)
{
	this->curX = x;
	this->curY = y;

	// Fix if outside boundaries
	if (this->curX < 0) this->curX = 0;
	else if (this->curX >= this->mapSize.width) this->curX = this->mapSize.width - 1;

	if (this->curY < 0) this->curY = 0;
	else if (this->curY >= this->mapSize.height) this->curY = this->mapSize.height - 1;

	// Calc new position
	Vec2 position = Vec2(this->curX, this->curY);

	this->stopAllActions();
	this->runAction(MoveTo::create(0.2f, position)); // Run action
}

void Cursor::moveToXY(Vec2 delta)
{
	this->moveToXY((int)delta.x, (int)delta.y);
}

int Cursor::getX()
{
	return this->curX;
}

int Cursor::getY()
{
	return this->curY;
}

Vec2 Cursor::getXY()
{
	return Vec2(this->curX, this->curY);
}

Vec2 Cursor::getPixelPosition()
{
	return Vec2(this->curX * this->tileSize.width, this->curY * this->tileSize.height);
}

int Cursor::getPixelPositionX()
{
	return (int)(this->curX * this->tileSize.width);
}

int Cursor::getPixelPositionY()
{
	return (int)(this->curY * this->tileSize.height);
}
