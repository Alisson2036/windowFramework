#pragma once
#include <Windows.h>


class Mouse
{
	friend class window;
public:
	Mouse() = default;
	Mouse(Mouse&) = delete;
	Mouse operator=(Mouse&) = delete;

	int getX() const;
	int getY() const;

	int getScroll() const;
	void resetScroll();

	bool leftButtonPressed();
	bool rightButtonPressed();

private:
	void updatePosition(POINTS p);
	void buttonDown(const char buttonArgs);
	void buttonUp(const char buttonArgs);

	void emptyButtonList();

private:
	int xPos = 0;
	int yPos = 0;

	char mouseButtons = 0;

	static const constexpr char leftButton = 0x2;
	static const constexpr char rightButton = 0x1;

	int scroll=0;
	
	
};