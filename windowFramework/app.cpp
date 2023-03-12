#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{
	while (win.update()) loop();

}

void app::loop()
{

	std::string title = std::to_string(win.getMousePointer()->getX());

	int x = win.getMousePointer()->getX();
	int y = win.getMousePointer()->getY();

	win.Gfx().fillScreen((float)x/800.0f, (float)y/600.0f, 0);

	win.Gfx().testando();

	win.setTitle(title);

}
