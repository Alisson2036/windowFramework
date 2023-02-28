#pragma once
#include <Windows.h>


class Mouse
{
public:
	Mouse(MSG* lastMessagePointer);

	int getX() const;
	int getY() const;

private:
	MSG* lastMessage;


};