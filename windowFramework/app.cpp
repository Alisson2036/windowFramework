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

	float r = (float)x / 800.0f;
	float g = (float)y / 600.0f;
	float b = 0;

	win.Gfx().fillScreen(r/2, g/2, b/2);

	float xPos = ((float)x - 400.0f) / 400.0f;
	float yPos = -((float)y - 300.0f) / 300.0f;

	title = std::to_string(xPos) + " " + std::to_string(yPos);

	r *= 255;
	g *= 255;
	b *= 255;

	vertex2d v[3] =
	{
		vertex2d( xPos, yPos, r, 255-r, g, 255),
		vertex2d( 0.5f,-0.5f, 255-g, r, 0, 255),
		vertex2d(-0.5f,-0.5f, g, 0, 255-r, 255),
	};

	Triangle t;
	t.setFill(win.Gfx().getFillable());
	t.create(v);

	win.Gfx().drawObject(t);
	//win.Gfx().test2();

	win.setTitle(title);

}
