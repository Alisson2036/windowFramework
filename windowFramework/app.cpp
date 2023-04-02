#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{
	Image img(L"a.png");
	tex.create(img);

	colorBlendTriangle::vertex2d v[] =
	{
		colorBlendTriangle::vertex2d(-1.0f,-1.0f,-1.0f, 255u, 0u  , 0u  , 255u),
		colorBlendTriangle::vertex2d( 1.0f,-1.0f,-1.0f, 0u  , 0u  , 255u, 255u),
		colorBlendTriangle::vertex2d(-1.0f, 1.0f,-1.0f, 0u  , 255u, 0u  , 255u),
		colorBlendTriangle::vertex2d( 1.0f, 1.0f,-1.0f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d(-1.0f,-1.0f, 1.0f, 70u , 0u  , 200u, 255u),
		colorBlendTriangle::vertex2d( 1.0f,-1.0f, 1.0f, 0u  , 0u  , 255u, 255u),
		colorBlendTriangle::vertex2d(-1.0f, 1.0f, 1.0f, 0u  , 255u, 0u  , 255u),
		colorBlendTriangle::vertex2d( 1.0f, 1.0f, 1.0f, 255u, 0u  , 255u, 255u),
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
	
	cube.create(win.Gfx().getPipeline(), v, ARRAYSIZE(v), i, ARRAYSIZE(i));

	//-----------------
	TexturedTriangle::vertex3d vp[] =
	{
		TexturedTriangle::vertex3d(-10.0f, 0.0f, 10.0f, 0.0f, 0.0f),//cima esquerda
		TexturedTriangle::vertex3d( 10.0f, 0.0f, 10.0f, 2.0f, 0.0f),//cima direita
		TexturedTriangle::vertex3d(-10.0f, 0.0f,-10.0f, 0.0f, 2.0f),//baixo esquerda
		TexturedTriangle::vertex3d( 10.0f, 0.0f,-10.0f, 2.0f, 2.0f) //baixo direita
	};
	short ip[] =
	{
		0,1,2, 1,3,2
	};

	plane.create(win.Gfx().getPipeline(), vp, ARRAYSIZE(vp), ip, ARRAYSIZE(ip), &tex);



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
	auto kb = win.getKeyboarPointer();
	if (kb->isKeyPressed('W')) cam.movePosition(0.0f, 0.0f, 0.1f);
	if (kb->isKeyPressed('S')) cam.movePosition(0.0f, 0.0f,-0.1f);
	if (kb->isKeyPressed('A')) cam.movePosition(-0.1f, 0.0f, 0.0f);
	if (kb->isKeyPressed('D')) cam.movePosition(0.1f, 0.0f, 0.0f);
	if (kb->isKeyPressed('R')) cam.movePosition(0.0f, 0.1f, 0.0f);
	if (kb->isKeyPressed('F')) cam.movePosition(0.0f,-0.1f, 0.0f);
	//olhar para outros angulos
	if (kb->isKeyPressed('I')) cam.moveAngle( 0.1f, 0.0f);
	if (kb->isKeyPressed('K')) cam.moveAngle(-0.1f, 0.0f);
	if (kb->isKeyPressed('J')) cam.moveAngle( 0.0f, 0.1f);
	if (kb->isKeyPressed('L')) cam.moveAngle( 0.0f,-0.1f);


	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);


	float pos[3] = { 0.0f,-2.0f,0.0f };
	float angle[3] = { 0.0f, 0.0f, 0.0f };

	Instance inst(pos, angle);

	plane.update(inst);
	plane.draw();

	angle[0] = DirectX::XM_PI;
	inst.update(pos, angle);
	plane.update(inst);
	plane.draw();

	for (int n = -5; n < -1; n++)
	{
		for (int j = -5; j < 5; j++)
		{
			for (int i = -20; i < 20; i++)
			{
				pos[0] = i * 4;
				pos[1] = cos(timeSinceCreation.getPassedSeconds() * 2 + i) + n * 10;
				pos[2] = sin(timeSinceCreation.getPassedSeconds() * 2 + i) + j * 10;

				inst.update(pos, angle);

				cube.update(inst);
				cube.draw();
			}
		}
	}
	
}
