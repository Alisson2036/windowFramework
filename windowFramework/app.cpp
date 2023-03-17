#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{

	cube1.setFill(win.Gfx().getFillable());
	cube2.setFill(win.Gfx().getFillable());
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

	
	cube1.create(v,ARRAYSIZE(v), i, ARRAYSIZE(i));
	cube2.create(v, ARRAYSIZE(v), i, ARRAYSIZE(i));


	while (win.update()) loop();

}

void app::loop()
{
	float x = win.getMousePointer()->getX();
	float y = win.getMousePointer()->getY();

	float xPos = ((float)x - 400.0f) / 400.0f;
	float yPos = -((float)y - 300.0f) / 300.0f;



	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);

	float pos[3]   = {xPos,yPos,0.0f};
	float angle[3] = {timeSinceCreation.getPassedSeconds(), timeSinceCreation.getPassedSeconds(), 0.0f};
	cube1.update(pos, angle);

	float pos2[3] = { 0.0f,0.0f,0.0f };
	float angle2[3] = { 0.0f, 0.0f, 0.0f };
	cube2.update(pos2, angle2);



	win.Gfx().drawObject(cube1);
	win.Gfx().drawObject(cube2);

}
