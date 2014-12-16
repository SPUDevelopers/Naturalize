#ifndef __CURSOR_H__
#define __CURSOR_H__

class Cursor
{
public:
	Cursor();
	~Cursor();

	// Moves the cursor relative to its current position.
	void move(int dx, int dy);

	// Moves the cursor to an absolute position on the map.
	void moveToXY(int x, int y);
};

#endif
