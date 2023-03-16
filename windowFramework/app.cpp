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
		vertex2d( 0.0f, 0.5f, 0.0f, 255, 0  , 0  , 255),
		vertex2d( 0.5f,-0.5f, 0.0f, 0  , 0  , 255, 255),
		vertex2d(-0.5f,-0.5f, 0.0f, 0  , 255, 0  , 255),
		vertex2d( 0.0f, 0.0f, 1.0f, 255, 255, 255, 255),
	};
	short i[] =
	{
		0,2,1,
		3,1,2,
		3,0,1,
		3,2,0
	};

	t.create(v,4,i,12);


	while (win.update()) loop();

}

void app::loop()
{

	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);

	
	t.update();
	win.Gfx().drawObject(t);


}
