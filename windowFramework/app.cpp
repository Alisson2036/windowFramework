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

	for (int i = 50; i < 200; i++)
		img.drawPixel(i, 50, color(255u, 0u, 255u, 255u));

	tex.create(img);

	//coloca luz na pipeline
	win.Gfx().getPipeline()->setLight(&light);
	//coloca camera na pipeline
	win.Gfx().getPipeline()->setCamera(&cam);

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


	cam.setPositionAndAngle({ 0.0f,0.0f,-4.0f }, { 0.0f,0.0f });
	
	//CRIA A IMAGEM ALEATORIA QUE FICA NA TELA
	img.fromRenderText(L"teste de texto", L"fonte", 200, 200, color(255u, 255u, 255u, 255u), 40);
	Texture ab;
	ab.create(img,false);
	plane.create(
		win.Gfx().getPipeline(),
		{
			{-1.0f,-1.0f},
			{0.5f,0.5f},
			{0.0f,0.0f},
			{1.0f,1.0f}
		}
		,&ab
	);

	while (win.update()) loop();

}

void app::loop()
{
	//posicao do mouse na tela 
	float x = (float)win.getMousePointer()->getX();
	float y = (float)win.getMousePointer()->getY();

	//posicao do mouse de acordo com as coordenadas no d3d
	float xPos = ((float)x - 400.0f) / 400.0f;
	float yPos = -((float)y - 300.0f) / 300.0f;

	//codigo para mecher a tela 
	if (win.getMousePointer()->rightButtonPressed())
	{
		win.showMouse(false);
		cam.moveAngle({ yPos, -xPos });


		win.setMousePosition(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2);
	}
	else win.showMouse(true);


	//movimento da camera
	auto kb = win.getKeyboarPointer();
	if (kb->isKeyPressed('W')) cam.movePosition({0.0f, 0.0f, 0.1f});
	if (kb->isKeyPressed('S')) cam.movePosition({0.0f, 0.0f,-0.1f});
	if (kb->isKeyPressed('A')) cam.movePosition({-0.1f, 0.0f, 0.0f});
	if (kb->isKeyPressed('D')) cam.movePosition({0.1f, 0.0f, 0.0f});
	if (kb->isKeyPressed('R')) cam.movePosition({0.0f, 0.1f, 0.0f});
	if (kb->isKeyPressed('F')) cam.movePosition({ 0.0f,-0.1f, 0.0f });

	if (kb->isKeyPressed('Z')) a+=0.1f;
	if (kb->isKeyPressed('X')) a-= 0.1f;


	//preenche a tela
	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);



	vec3 pos = { 0.0f, 0.0f, 0.0f };
	vec3 angle = { 0.0f, 0.0f, 0.0f};



	Instance inst(pos, angle);



	//desenha o objeto texturizado no meio da tela
	cubeTex.update(inst);
	
	for (int y = -10; y < 10; y++)
	{
		for (int x = -10; x < 10; x++)
		{
			pos.x = x * 2.0f;
			pos.y = -3.0f + (float)cos(x) + (float)cos(y);
			pos.z = y*2.0f;


			inst.update(pos, angle);

			cubeTex.update(inst);
			cubeTex.draw();
		}
	}


	//muda posicao da luz 
	light.updatePos({2.0f, a, 0.0f});
	cubeLight.update({ { 2.0f,a,0.0f }, angle });
	cubeLight.draw();

	
	//cria e desenha todos os cubos coloridos
	for (int n = 1; n < 5; n++)
	{
		for (int j = -5; j < 5; j++)
		{
			for (int i = -20; i < 20; i++)
			{
				pos.x = i * 4.0f;
				pos.y = cos(timeSinceCreation.getPassedSeconds() * 2 + i) + n * 10;
				pos.z = sin(timeSinceCreation.getPassedSeconds() * 2 + i) + j * 10;

				inst.update(pos, angle);

				cube.update(inst);
				cube.draw();
			}
		}
	}
	plane.draw();
}
