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
	vertex2d v[] =
	{
		vertex2d(-1.0f,-1.0f,-1.0f, 255, 0  , 0  , 255),
		vertex2d( 1.0f,-1.0f,-1.0f, 0  , 0  , 255, 255),
		vertex2d(-1.0f, 1.0f,-1.0f, 0  , 255, 0  , 255),
		vertex2d( 1.0f, 1.0f,-1.0f, 255, 255, 255, 255),
		vertex2d(-1.0f,-1.0f, 1.0f, 70 , 0  , 200, 255),
		vertex2d( 1.0f,-1.0f, 1.0f, 0  , 0  , 255, 255),
		vertex2d(-1.0f, 1.0f, 1.0f, 0  , 255, 0  , 255),
		vertex2d( 1.0f, 1.0f, 1.0f, 255, 0  , 255, 255),
	};
	short i[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	t.create(v,ARRAYSIZE(v), i, ARRAYSIZE(i));


	while (win.update()) loop();

}

void app::loop()
{

	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);

	
	t.update();
	win.Gfx().drawObject(t);


}
