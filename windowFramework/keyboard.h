#pragma once
#include <Windows.h>
#include <vector>

class Keyboard
{
	friend class window;
public:
	Keyboard() = default;
	Keyboard(Keyboard&) = delete;
	Keyboard operator=(Keyboard&) = delete;

	bool isKeyPressed(char key);
	

private:
	void keyDown(char key);
	void keyUp(char key);
	void emptyKeys();

private:
	std::vector<char> keysPressed;
	


};