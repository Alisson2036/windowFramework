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
	
	//-----------------------------

	objLoader obj;
	obj.fromFile("cube.obj");

	//carrega o shader
	texturedShader.create(L"texturedVS.cso", L"texturedPS.cso");

	//cria o cubo teste
	std::vector<vec3> verArr =
	{
		vec3(-1.0f,-1.0f,-1.0f),
		vec3(1.0f,-1.0f,-1.0f),
		vec3(-1.0f, 1.0f,-1.0f),
		vec3(1.0f, 1.0f,-1.0f),
		vec3(-1.0f,-1.0f, 1.0f),
		vec3(1.0f,-1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
	};
	std::vector<color> colors =
	{
		color(255u, 0u  , 0u  , 255u),
		color(0u  , 0u  , 255u, 255u),
		color(0u  , 255u, 0u  , 255u),
		color(255u, 255u, 255u, 255u),
		color(70u , 0u  , 200u, 255u),
		color(0u  , 0u  , 255u, 255u),
		color(0u  , 255u, 0u  , 255u),
		color(255u, 0u  , 255u, 255u),
	};
	std::vector<color> colorsWhite =
	{
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
		color(255u, 255u, 255u, 255u),
	};
	std::vector<int> ind =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};


	colorBlendShader.create(L"colorBlendVS.cso", L"colorBlendPS.cso");

	colorBlendCube.create(colorBlendShader);
	colorBlendCube.loadFromVertexArray(verArr);
	colorBlendCube.loadFromColorArray(colors);
	colorBlendCube.setVertexIndices(ind);
	colorBlendCube.lock();

	cubeLight.create(colorBlendShader);
	cubeLight.loadFromVertexArray(verArr);
	cubeLight.loadFromColorArray(colorsWhite);
	cubeLight.setVertexIndices(ind);
	cubeLight.lock();
	cubeLight.setScale({ 0.2f, 0.2f, 0.2f });

	//cria o cubo texturizado
	texturedCube.create(texturedShader);
	texturedCube.loadFromObj(obj);
	texturedCube.setTexture(&tex, 0);
	texturedCube.lock();

	//move camera para posicao inicial
	cam.setPositionAndAngle({ 0.0f,0.0f,-4.0f }, { 0.0f,0.0f });
	
	//carrega a fonte
	fonte = new Image::font(L"Times New Roman", 80.0f);

	//CRIA A IMAGEM ALEATORIA QUE FICA NA TELA
	img.fromRenderText(L"oi tudo bem?", *fonte, 400, 300, color(255u, 255u, 255u, 255u));
	Texture ab;
	ab.create(img,false);
	plane.create(ab);


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



	Position3d inst(pos, angle);



	//desenha o objeto texturizado no meio da tela
	//cubeTex.update(inst);
	
	for (int y = -10; y < 10; y++)
	{
		for (int x = -10; x < 10; x++)
		{
			pos.x = x * 2.0f;
			pos.y = -3.0f + (float)cos(x) + (float)cos(y);
			pos.z = y*2.0f;


			texturedCube.set(pos, angle);

			//texturedCube.update(inst);
			//texturedCube.draw();
			win.Gfx().getPipeline()->bind(texturedCube);
		}
	}


	//muda posicao da luz 
	light.updatePos({2.0f, a, 0.0f});
	cubeLight.set({ 2.0f,a,0.0f }, angle);
	win.Gfx().getPipeline()->bind(cubeLight);


	
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

				colorBlendCube.set(pos, { 0.f,0.f,0.f });
				win.Gfx().getPipeline()->bind(colorBlendCube);
			}
		}
	}

	//DESENHA A IMAGEM ALEATORIA QUE FICA NA TELA
	
	plane.setPosition({ -1.0f, 1.0f });
	plane.draw(*win.Gfx().getPipeline());
}
