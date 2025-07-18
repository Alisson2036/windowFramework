#pragma once
#include <Windows.h>

// Gerencia um controlador de mouse que lida com a entrada do mouse,
// incluindo posi��o, bot�es pressionados e rolagem.
class Mouse
{
public:
	Mouse() = default;
	Mouse(Mouse&) = delete;
	Mouse operator=(Mouse&) = delete;

	int getX() const;
	int getY() const;

	int getRawX() const;
	int getRawY() const;
	void resetRaw();

	int getScroll() const;
	void resetScroll();

	bool leftButtonPressed();
	bool rightButtonPressed();

public:
	void updatePosition(POINTS p);
	void updateRawPosition(int x, int y);
	void buttonDown(const char buttonArgs);
	void buttonUp(const char buttonArgs);

	void wheelMove(int delta);

	void emptyButtonList();

	static const constexpr char leftButton = 0x2;
	static const constexpr char rightButton = 0x1;
private:
	int xPos = 0;
	int yPos = 0;

	int rawX = 0;
	int rawY = 0;

	char mouseButtons = 0;


	float scroll=0;
	
	
};