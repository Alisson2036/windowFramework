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
	return scroll;
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

