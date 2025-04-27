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
	Image bricks(L"bricks.jpg");
	Image bricksNormal(L"bricksNormal.jpg");
	Image solidWhite;
	solidWhite.fromBlank(100, 100);
	solidWhite.drawRectangle(
		vec2(0, 0),
		vec2(100, 100),
		color(255u, 255u, 255u, 255u)
	);

	

	tex.createWithMipMap(img);
	brickTex.createWithMipMap(bricks);
	brickTexNormal.createWithMipMap(bricksNormal);
	solidWhiteTex.create(solidWhite);

	//coloca luz na pipeline
	win.Gfx().getPipeline()->setLight(&light);
	//coloca camera na pipeline
	win.Gfx().getPipeline()->setCamera(&cam);
	
	//-----------------------------

	objLoader obj;
	objLoader planeObj;
	objLoader sphereObj;
	obj.fromFile("cube.obj");
	planeObj.fromFile("plane.obj");
	sphereObj.fromFile("sphere.obj");

	//carrega o shader
	texturedShader.create(L"texturedVS.cso", L"texturedPS.cso");
	normalShader.create(L"texturedVS.cso", L"normalPS.cso");

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


	//cria o cubo bricks
	normalCube.create(normalShader);
	normalCube.loadFromObj(obj);
	normalCube.setTexture(&brickTex, 0);
	normalCube.setTexture(&brickTexNormal, 1);
	normalCube.lock();

	//cria a esfera
	sphere.create(texturedShader);
	sphere.loadFromObj(sphereObj);
	sphere.setTexture(&solidWhiteTex, 0);
	sphere.lock();
	

	//move camera para posicao inicial
	cam.setPositionAndAngle({ 0.0f,4.0f,-12.0f }, { 0.0f,0 });
	
	//carrega a fonte
	fonte = new Image::font(L"Times New Roman", 40.0f);

	//CRIA A IMAGEM ALEATORIA QUE FICA NA TELA
	img.fromRenderText(L"oi tudo bem?", *fonte, 800, 600, color(255u, 255u, 255u, 255u));
	hudObject.create(img);

	//inicializa a imagem do hud
	hud.fromBlank(800, 600);

	//adiciona objetos de fisica
	phyObjs.push_back(physicsObject(vec3(0.0f,6.0f,0.0f)));
	phyDomain.addObject(&phyObjs[0]);
	phyDomain.setGravity(vec3(0.0f, -10.0f, 0.0f));


	while (win.update()) loop();

}

void app::loop()
{
	//posicao do mouse na tela 
	float x = (float)win.getMousePointer()->getX();
	float y = (float)win.getMousePointer()->getY();


	//codigo para mecher a tela 
	static bool lastRightButtonState = false;
	static vec2 lastMousePos;
	if (win.getMousePointer()->rightButtonPressed() && lastRightButtonState)
	{
		win.showMouse(false);

		static const float sens = 0.003f;
		float xRaw = (float)win.getMousePointer()->getRawX();
		float yRaw = (float)win.getMousePointer()->getRawY();
		cam.moveAngle({ -yRaw * sens, -xRaw * sens });
		win.getMousePointer()->resetRaw();


		//win.setMousePosition(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2);
		win.setMousePosition(lastMousePos.x, lastMousePos.y);
	}
	else if (win.getMousePointer()->rightButtonPressed())
	{
		lastRightButtonState = true;
		win.getMousePointer()->resetRaw();
		lastMousePos.x = x;
		lastMousePos.y = y;
	}
	else 
	{
		win.showMouse(true);
		lastRightButtonState = false;
	}


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

	//physics logics
	phyDomain.solve(0.01);


	//preenche a tela
	win.Gfx().fillScreen(0.1f, 0.4f, 0.7f);


	vec3 pos = { 0.0f, 0.0f, 0.0f };
	vec3 angle = { 0.0f, 0.0f, 0.0f};

	//cria o chao
	for (int y = -10; y < 10; y++)
	{
		for (int x = -10; x < 10; x++)
		{
			pos.x = x * 2.0f;
			pos.y = -1.0f; //+ (float)cos(x) + (float)cos(y);
			pos.z = y*2.0f;


			normalCube.set(pos, angle);

			//texturedCube.update(inst);
			//texturedCube.draw();
			win.Gfx().getPipeline()->bind(normalCube);
		}
	}


	//muda posicao da luz 
	light.updatePos({ 2.0f + 3, a, 0.0f});
	cubeLight.set({ 2.0f + 3,a,0.0f}, angle);
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

	//coloca o cubo texturizado
	texturedCube.set({ 10.f,1.f,-3.f }, { 0.f, 0.f, 0.f });
	win.Gfx().getPipeline()->bind(texturedCube);

	//coloca as esferas
	for (auto& i : phyObjs)
	{
		sphere.set(i.getPosition(), {0.f, 0.f, 0.f});
		win.Gfx().getPipeline()->bind(sphere);
	}


	//escreve texto do frametime
	static float dTime;
	float frameTime = (timeSinceCreation.getPassedSeconds() - dTime) * 1000.0f;
	dTime = timeSinceCreation.getPassedSeconds();


	hud.drawText(
		std::to_wstring(frameTime),
		*fonte,
		vec2(0, 0),
		color(255u, 255u, 255u, 255u)
	);
	
	//desenha e atualiza o hud
	hudObject.update(hud);
	hudObject.draw(*win.Gfx().getPipeline());
	hud.clear();
}
