#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 800, 600),
	timeSinceCreation()
{

}

void app::start()
{
	//carregando imagem
	Image img(L"a.png");
	tex.create(img);

	//coloca luz na pipeline
	win.Gfx().getPipeline()->setLight(&light);

	//cria o cubo
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
	
	//cria o cubo para mostrar a luz
	//cria o cubo
	colorBlendTriangle::vertex2d v2[] =
	{
		colorBlendTriangle::vertex2d(-0.1f,-0.1f,-0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d( 0.1f,-0.1f,-0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d(-0.1f, 0.1f,-0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d( 0.1f, 0.1f,-0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d(-0.1f,-0.1f, 0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d( 0.1f,-0.1f, 0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d(-0.1f, 0.1f, 0.1f, 255u, 255u, 255u, 255u),
		colorBlendTriangle::vertex2d( 0.1f, 0.1f, 0.1f, 255u, 255u, 255u, 255u)
	};
	cubeLight.create(win.Gfx().getPipeline(), v2, ARRAYSIZE(v2), i, ARRAYSIZE(i));
	
	//-----------------------------

	objLoader obj;
	obj.fromFile("cube.obj");
	cubeTex.create(win.Gfx().getPipeline(), obj, &tex);


	float p[3] = {0.0f,0.0f,-4.0f};
	float a[3] = {0.0f,0.0f,0.0f};
	cam.setPositionAndAngle(p, a);
	
	Instance::initializeCamera(&cam);


	while (win.update()) loop();

}

void app::loop()
{
	float x = (float)win.getMousePointer()->getX();
	float y = (float)win.getMousePointer()->getY();

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


	//preenche a tela
	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);



	float pos[3] = { 0.0f,-2.0f,0.0f };
	float angle[3] = { 0.0f, timeSinceCreation.getPassedSeconds(), 0.0f};



	Instance inst(pos, angle);



	//desenha o objeto texturizado no meio da tela
	cubeTex.update(inst);
	cubeTex.draw();



	//muda posicao da luz 
	float pos1[3] = { 2.0f,-2.0f,0.0f };
	light.updatePos(pos1);
	cubeLight.update(Instance(pos1, angle));
	cubeLight.draw();


	//cria e desenha todos os cubos
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
