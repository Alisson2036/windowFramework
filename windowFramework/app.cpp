#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{

	cube.setFill(win.Gfx().getFillable());
	vertex2d v[] =
	{
		vertex2d(-1.0f,-1.0f,-1.0f, 255u, 0u  , 0u  , 255u),
		vertex2d( 1.0f,-1.0f,-1.0f, 0u  , 0u  , 255u, 255u),
		vertex2d(-1.0f, 1.0f,-1.0f, 0u  , 255u, 0u  , 255u),
		vertex2d( 1.0f, 1.0f,-1.0f, 255u, 255u, 255u, 255u),
		vertex2d(-1.0f,-1.0f, 1.0f, 70u , 0u  , 200u, 255u),
		vertex2d( 1.0f,-1.0f, 1.0f, 0u  , 0u  , 255u, 255u),
		vertex2d(-1.0f, 1.0f, 1.0f, 0u  , 255u, 0u  , 255u),
		vertex2d( 1.0f, 1.0f, 1.0f, 255u, 0u  , 255u, 255u),
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

	
	cube.create(v, ARRAYSIZE(v), i, ARRAYSIZE(i));

	float p[3] = {0.0f,0.0f,-4.0f};
	float a[3] = {0.0f,0.0f,0.0f};
	cam.setPositionAndAngle(p, a);
	
	Instance::initializeCamera(&cam);


	while (win.update()) loop();

}

void app::loop()
{
	float x = (int)win.getMousePointer()->getX();
	float y = (int)win.getMousePointer()->getY();

	float xPos = ((float)x - 400.0f) / 400.0f;
	float yPos = -((float)y - 300.0f) / 300.0f;

	//movimento da camera
	if (win.getKeyboarPointer()->isKeyPressed('W')) cam.movePosition(0.0f, 0.0f, 0.1f);
	if (win.getKeyboarPointer()->isKeyPressed('S')) cam.movePosition(0.0f, 0.0f,-0.1f);
	if (win.getKeyboarPointer()->isKeyPressed('A')) cam.movePosition(-0.1f, 0.0f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('D')) cam.movePosition(0.1f, 0.0f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('R')) cam.movePosition(0.0f, 0.1f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('F')) cam.movePosition(0.0f,-0.1f, 0.0f);
	//olhar para outros angulos
	if (win.getKeyboarPointer()->isKeyPressed('I')) cam.moveAngle(0.1f, 0.0f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('K')) cam.moveAngle(-0.1f, 0.0f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('J')) cam.moveAngle(0.0f, 0.1f, 0.0f);
	if (win.getKeyboarPointer()->isKeyPressed('L')) cam.moveAngle(0.0f,-0.1f, 0.0f);


	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);


	float pos[3] = { 0.0f,0.0f,0.0f };
	float angle[3] = { 0.0f, 0.0f, 0.0f };

	Instance inst(pos, angle);
	for (int j = 0; j < 10; j++)
	{
		for (int i = -20; i < 20; i++)
		{
			pos[0] = i * 4;
			pos[1] = cos(timeSinceCreation.getPassedSeconds() * 2 + i);
			pos[2] = sin(timeSinceCreation.getPassedSeconds() * 2 + i) + j*10;

			inst.update(pos, angle);

			cube.update(inst);
			cube.draw();
		}
	}

}
