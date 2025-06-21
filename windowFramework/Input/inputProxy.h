#pragma once
#include "mouse.h"
#include "keyboard.h"


class InputProxy
{
public:
	InputProxy(Mouse* _mouse, Keyboard* _keyboard);
	InputProxy(InputProxy&) = delete;
	InputProxy operator=(auto&) = delete;
	
	//--MOUSE--

	int mouseX() const;
	int mouseY() const;

	int rawMouseX() const;
	int rawMouseY() const;
	void resetRaw();

	int scroll() const;
	void resetScroll();

	bool leftButtonPressed() const;
	bool rightButtonPressed() const;


	//--KEYBOARD--

	bool isKeyPressed(char key) const;
	char getLastKey() const;

private:
	Mouse* mouse;
	Keyboard* keyboard;

};