#include "app.h"

app::app():
	win(L"Jojo fofo", 800, 600)
{

}

void app::start()
{
	while (win.update()) loop();

	throw frameworkException(__LINE__, __FILE__);
}

void app::loop()
{

	std::string title = std::to_string(win.getMousePointer()->getX());

	win.setTitle(title);

}
