#include "keyboard.h"

bool Keyboard::isKeyPressed(char key)
{
    for (auto i : keysPressed) if (i == key) return true;
    return false;
}

void Keyboard::keyDown(char key)
{
    keysPressed.push_back(key);
}

void Keyboard::keyUp(char key)
{
    for (int i = 0; i < keysPressed.size(); i++) 
        if (keysPressed[i] == key) keysPressed.erase(keysPressed.begin() + i);
}

void Keyboard::emptyKeys()
{
    keysPressed.clear();
}
