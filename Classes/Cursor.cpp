#include "Cursor.h"

Cursor::Cursor()
{
	curX = 0;
	curY = 0;
}

Cursor::~Cursor()
{
}

void Cursor::move(int dx, int dy)
{
	curX += dx * 128;
	curY += dy * 128;
}

void Cursor::moveToXY(int x, int y)
{
	curX = x * 128;
	curY = y * 128;
}

int Cursor::getX()
{
	return curX;
}

int Cursor::getY()
{
	return curY;
}