#include "inputProxy.h"

InputProxy::InputProxy(Mouse* _mouse, Keyboard* _keyboard)
	:
	mouse(_mouse),
	keyboard(_keyboard)
{
}

int InputProxy::mouseX() const
{
	return mouse->getX();
}

int InputProxy::mouseY() const
{
	return mouse->getY();
}

int InputProxy::rawMouseX() const
{
	return mouse->getRawX();
}

int InputProxy::rawMouseY() const
{
	return mouse->getRawY();
}

void InputProxy::resetRaw()
{
	mouse->resetRaw();
}

int InputProxy::scroll() const
{
	return mouse->getScroll();
}

void InputProxy::resetScroll()
{
	mouse->resetScroll();
}

bool InputProxy::leftButtonPressed() const
{
	return mouse->leftButtonPressed();
}

bool InputProxy::rightButtonPressed() const
{
	return mouse->rightButtonPressed();
}

bool InputProxy::isKeyPressed(char key) const
{
	return keyboard->isKeyPressed(key);
}

char InputProxy::getLastKey() const
{
	return keyboard->getLastKey();
}
