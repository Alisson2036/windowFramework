#include "mouse.h"


int Mouse::getX() const
{
	return xPos;
}

int Mouse::getY() const
{
	return yPos;
}

int Mouse::getScroll() const
{
	return (int)scroll;
}

void Mouse::resetScroll()
{
	scroll = 0;
}

bool Mouse::leftButtonPressed()
{
	return mouseButtons & leftButton;
}

bool Mouse::rightButtonPressed()
{
	return mouseButtons & rightButton;
}

void Mouse::updatePosition(POINTS p)
{
	xPos = p.x;
	yPos = p.y;

}

void Mouse::buttonDown(const char buttonArgs)
{
	mouseButtons = mouseButtons | buttonArgs;
}

void Mouse::buttonUp(const char buttonArgs)
{
	mouseButtons = mouseButtons - buttonArgs;
}

void Mouse::wheelMove(int delta)
{
	scroll += (float)delta / 120.0f;
}

void Mouse::emptyButtonList()
{
	mouseButtons = 0;
}

