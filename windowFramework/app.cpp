#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{

	t.setFill(win.Gfx().getFillable());
	vertex2d v[3] =
	{
		vertex2d(0.0f, 0.5f, 255, 0  , 0  , 255),
		vertex2d(0.5f,-0.5f, 0  , 0  , 255, 255),
		vertex2d(-0.5f,-0.5f, 0  , 255, 0  , 255),
	};

	t.create(v);


	while (win.update()) loop();

}

void app::loop()
{

	win.Gfx().fillScreen(0, 0, 0);

	
	t.update();
	win.Gfx().drawObject(t);
	//win.Gfx().test2();


}
