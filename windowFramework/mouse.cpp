#include "mouse.h"

Mouse::Mouse(MSG* lastMessagePointer):
	lastMessage(lastMessagePointer)
{	
}

int Mouse::getX() const
{
	return lastMessage->pt.x;
}

int Mouse::getY() const
{
	return lastMessage->pt.y;
}

