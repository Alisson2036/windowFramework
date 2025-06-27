#pragma once
#include <Windows.h>
#include <vector>

// Gerenciamento de entrada do teclado.
class Keyboard
{
public:
	Keyboard() = default;
	Keyboard(Keyboard&) = delete;
	Keyboard operator=(Keyboard&) = delete;

	bool isKeyPressed(char key);
	char getLastKey();

public:
	void keyDown(char key);
	void keyUp(char key);
	void emptyKeys();

private:
	std::vector<char> keysPressed;

};