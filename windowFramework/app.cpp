#include "app.h"

app::app()
	:
	win(L"Jojo fofo", 1200, 900),
	timeSinceCreation()
{
	pipeline = win.Gfx().getPipeline();

	//criando novo target view
	newTarget.create(vec2(200, 200));
	newDTTarget.create(vec2(600, 600));

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
	pipeline->setLight(&light);
	//coloca camera na pipeline
	pipeline->setCamera(&cam);

	//-----------------------------

	objLoader obj;
	objLoader planeObj;
	objLoader sphereObj;
	objLoader waterObj;
	obj.fromFile("cube.obj");
	planeObj.fromFile("plane.obj");
	sphereObj.fromFile("sphere.obj");
	waterObj.fromFile("waterPlane.obj");

	//carrega o shader
	texturedShader.create(L"texturedVS.cso", L"texturedPS.cso");
	texturedInstancedShader.create(L"texturedInstancedVS.cso", L"texturedPS.cso");
	normalShader.create(L"texturedInstancedVS.cso", L"normalPS.cso");
	waterShader.create(L"waterVS.cso", L"waterPS.cso");

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
	texturedCube.create(texturedInstancedShader);
	texturedCube.loadFromObj(obj);
	//texturedCube.setTexture(&tex, 0);
	texturedCube.setTexture(newTarget.getTexture(), 0);
	texturedCube.lock();
	std::vector<vec3> positions =
	{
		vec3(1.f,1.f,1.f),
		vec3(2.f,4.f,1.f),
		vec3(3.f,1.f,3.f),
	};
	texturedCube.setInstancesPos(positions);
	positions = {};

	//cria o cubo bricks
	{
		normalCube.create(normalShader);
		normalCube.loadFromObj(obj);
		normalCube.setTexture(&brickTex, 0);
		normalCube.setTexture(&brickTexNormal, 1);
		normalCube.setTexture(newDTTarget.getTexture(), 2);
		normalCube.lock();
		vec3 pos;
		for (int y = -10; y < 10; y++)
		{
			for (int x = -10; x < 10; x++)
			{
				pos.x = x * 2.0f;
				pos.y = -1.0f; //+ (float)cos(x) + (float)cos(y);
				pos.z = y * 2.0f;


				//normalCube.set(pos, vec3(0.f,0.f,0.f));
				positions.push_back(pos);
				//texturedCube.update(inst);
				//texturedCube.draw();
				//win.Gfx().getPipeline()->bind(normalCube);
			}
		}
		normalCube.setInstancesPos(positions);
	}

	//cria a water
	water.create(waterShader);
	water.loadFromObj(waterObj);
	water.lock();
	water.set(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	water.setScale(vec3(100, 1, 100));
	{
		float a = 0.0f;
		timerBuffer.create(&a, 1, sizeof(float) * 8);
		timerVertexBuffer.create(&a, 1, sizeof(float) * 8);
	}

	//cria a esfera
	sphere.create(texturedShader);
	sphere.loadFromObj(sphereObj);
	sphere.setTexture(&solidWhiteTex, 0);
	sphere.lock();


	//move camera para posicao inicial
	cam.setScreenProportion((float)win.getWindowSizeY() / (float)win.getWindowSizeX());
	cam.setPositionAndAngle({ 0.0f,4.0f,-12.0f }, { 0.0f,0 });

	//CAMERA LUZ
	lightCam.setViewSize(vec2(30, 30));
	lightCam.setPerspective(false);
	lightCam.setPositionAndAngle(vec3(0.f, 10.f, 0.f), vec2(-DirectX::XM_PIDIV2, 0.0f));
	light.setLightCam(&lightCam);

	//carrega a fonte
	fonte = new Image::font(L"Times New Roman", 40.0f);

	//inicializa a imagem do hud
	hud.fromBlank(win.getWindowSizeX(), win.getWindowSizeY());
	hudObject.create(hud);

	//adiciona objetos de fisica
	phyObjs.push_back(new physicsObject(vec3(1.0f, 4.0f, 0.0f)));
	phyObjs.push_back(new physicsObject(vec3(0.0f, 6.0f, 1.0f)));
	phyObjs.push_back(new physicsObject(vec3(0.0f, 8.0f, 0.0f)));

	for (auto& i : phyObjs)
		phyDomain.addObject(i);
	phyDomain.setGravity(vec3(0.0f, -10.0f, -0.0f));

	newTarget.clear();

}


void app::loop()
{

	//posicao do mouse na tela 
	float x = (float)win.getMousePointer()->getX();
	float y = (float)win.getMousePointer()->getY();


	std::vector<vec3> positions =
	{
		vec3(1.f,1.f,1.f),
		vec3(2.f,-1.f,1.f),
		vec3(3.f,1.f,3.f),
	};
	texturedCube.setInstancesPos(positions);

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


	//if (kb->isKeyPressed('P'))
	//{
	//	newTarget.clear();
	//	newDTTarget.clear();
	//	pipeline->setRenderTarget(nullptr, &newDTTarget);
	//}
	//else
	//{
	//	win.Gfx().drawToScreen();
	//}
	//newTarget.clear();

	//cria mais bolas
	static float lastBallTime = timeSinceCreation.getPassedSeconds();
	if (phyObjs.size() < 200 && timeSinceCreation.getPassedSeconds() > lastBallTime + 0.0f)
	{
		phyObjs.push_back(new physicsObject(vec3(4*cos(lastBallTime*1234.f), 15.0f, 4*sin(lastBallTime*78347.f))));
		phyDomain.addObject(phyObjs.back());

		lastBallTime = timeSinceCreation.getPassedSeconds();
	}

	hud.drawText(
		L"Objetos:" + std::to_wstring(phyObjs.size()),
		*fonte,
		vec2(0, 40),
		color(255, 255, 255, 255)
	);

	//physics logics
	phyDomain.solve(frameTime);


	//preenche a tela
	win.Gfx().fillScreen(0.2f, 0.6f, 0.9f);

	//depth render das esferas
	newDTTarget.clear();
	pipeline->setRenderTarget(nullptr, &newDTTarget);
	pipeline->setCamera(&lightCam);
	for (auto& i : phyObjs)
	{
		sphere.set(i->getPosition(), { 0.f, 0.f, 0.f });
		win.Gfx().getPipeline()->drawObject(sphere);
	}

	//render normal das esferas
	win.Gfx().drawToScreen();
	pipeline->setCamera(&cam);
	for (auto& i : phyObjs)
	{
		sphere.set(i->getPosition(), { 0.f, 0.f, 0.f });
		win.Gfx().getPipeline()->drawObject(sphere);
	}



	vec3 pos = { 0.0f, 0.0f, 0.0f };
	vec3 angle = { 0.0f, 0.0f, 0.0f};

	//cria o chao
	win.Gfx().getPipeline()->drawObject(normalCube);

	//muda posicao da luz 
	light.updatePos({ 2.0f + 3, a, 0.0f});
	cubeLight.set({ 2.0f + 3,a,0.0f}, angle);
	win.Gfx().getPipeline()->drawObject(cubeLight);


	
	//cria e desenha todos os cubos coloridos
	for (int n = 3; n < 4; n++)
	{
		for (int j = -5; j < 5; j++)
		{
			for (int i = -20; i < 20; i++)
			{
				pos.x = i * 4.0f;
				pos.y = cos(timeSinceCreation.getPassedSeconds() * 2 + i) + n * 10;
				pos.z = sin(timeSinceCreation.getPassedSeconds() * 2 + i) + j * 10;

				colorBlendCube.set(pos, { 0.f,0.f,0.f });
				win.Gfx().getPipeline()->drawObject(colorBlendCube);
			}
		}
	}

	//coloca o cubo texturizado
	texturedCube.set({ 10.f,1.f,-3.f }, { 0.f, 0.f, 0.f });
	texturedCube.setTexture(newDTTarget.getTexture(), 0);
	pipeline->drawObject(texturedCube);



	////escreve texto do frametime
	static float dTime;
	frameTime = (timeSinceCreation.getPassedSeconds() - dTime);
	dTime = timeSinceCreation.getPassedSeconds();

	////desenha agua
	float time = timeSinceCreation.getPassedSeconds();
	timerBuffer.update(&time);
	timerBuffer.setSlot(2);
	timerBuffer.bind();
	timerVertexBuffer.update(&time);
	timerVertexBuffer.setSlot(2);
	timerVertexBuffer.bind();
	win.Gfx().getPipeline()->drawObject(water);

	hud.drawText(
		std::to_wstring(frameTime * 1000.0f),
		*fonte,
		vec2(0, 0),
		color(255u, 255u, 255u, 255u)
	);
	
	//desenha e atualiza o hud
	hudObject.update(hud);
	hudObject.draw(*win.Gfx().getPipeline());
	hud.clear();
}



void app::start()
{


	while (win.update()) loop();

}