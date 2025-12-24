#include "app.h"

app::app()
	:
	eng(1400, 900),
	timeSinceCreation()
{
	pipeline = eng.getPipeline();
	factory = std::make_unique<ComponentFactory>(pipeline->getRegistry());

	//criando novo target view
	{
		//resolucao com -200 no x por causa do painel
		resolution3d = vec2(
			eng.getScreenSize().x - 200.0f, 
			eng.getScreenSize().y
		);
		target.create(resolution3d);
		targetDS.create(resolution3d);
		target.clear();
		targetDS.clear();
	}


	//assetManager.CreateAsset<IAsset>("TextureAsset", "Textures\\a.png");

	//criacao do shadowmap
	shadowMap.create(vec2(1200, 1200));


	//coloca luz na pipeline
	pipeline->setLight(&light);
	//coloca camera na pipeline
	pipeline->setCamera(&cam);


	//-----Asset Loading------

	// Meshes
	auto* cubeObj = assetManager.CreateAsset<MeshAsset>("Cube", "Objs\\cube.obj");
	auto* sphereObj = assetManager.CreateAsset<MeshAsset>("Sphere", "Objs\\sphere.obj");
	auto* waterObj = assetManager.CreateAsset<MeshAsset>("Water", "Objs\\waterPlane.obj");
	auto* coloredCube = assetManager.CreateAsset<MeshAsset>("ColoredCube");
	auto* whiteCube = assetManager.CreateAsset<MeshAsset>("whiteCube");

	// Textures
	auto* tex = assetManager.CreateAsset<TextureAsset>("a", L"Textures\\a.png", false);
	auto* brickTex = assetManager.CreateAsset<TextureAsset>("Bricks", L"Textures\\bricks.jpg", true);
	auto* brickTexNormal = assetManager.CreateAsset<TextureAsset>("BricksNormal", L"Textures\\bricksNormal.jpg", true);

	// Shaders
	auto* normalShader = assetManager.CreateAsset<ShaderAsset>(
		"normalShader",
		"Shaders\\texturedInstancedVS.hlsl",
		"CompiledShaders\\texturedInstancedVS.cso",
		"Shaders\\normalPS.hlsl",
		"CompiledShaders\\normalPS.cso"
		);
	auto* texturedShaderAsset = assetManager.CreateAsset<ShaderAsset>(
		"texturedShader",
		"Shaders\\texturedVS.hlsl",
		"CompiledShaders\\texturedVS.cso",
		"Shaders\\texturedPS.hlsl",
		"CompiledShaders\\texturedPS.cso"
	);

	// Load
	assetManager.LoadAll();


	// Cria textura de branco solido
	Image solidWhite;
	solidWhite.fromBlank(100, 100);
	solidWhite.drawRectangle(
		vec2(0, 0),
		vec2(100, 100),
		color(255u, 255u, 255u, 255u)
	);
	solidWhiteTex.create(solidWhite);



	//carrega os shaders
	texturedShader.create(L"CompiledShaders\\texturedVS.cso", L"CompiledShaders\\texturedPS.cso");
	texturedInstancedShader.create(L"CompiledShaders\\texturedInstancedVS.cso", L"CompiledShaders\\texturedPS.cso");
	//normalShader.create(L"CompiledShaders\\texturedInstancedVS.cso", L"CompiledShaders\\normalPS.cso");
	waterShader.create(L"CompiledShaders\\waterVS.cso", L"CompiledShaders\\waterPS.cso");
	colorBlendShader.create(L"CompiledShaders\\colorBlendVS.cso", L"CompiledShaders\\colorBlendPS.cso");

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

	//carrega vetores
	coloredCube->manualLoad("Position", verArr);
	coloredCube->manualLoad("Color", colors);
	whiteCube->manualLoad("Position", verArr);
	whiteCube->manualLoad("Color", colorsWhite);


	// Cria os cubos procedurais
	colorBlendCube.create(&colorBlendShader);
	colorBlendCube.load(coloredCube);
	colorBlendCube.setVertexIndices(ind);
	colorBlendCube.lock();

	cubeLight.create(&colorBlendShader);
	cubeLight.load(whiteCube);
	cubeLight.setVertexIndices(ind);
	cubeLight.lock();
	cubeLight.setScale({ 0.2f, 0.2f, 0.2f });

	//cria o cubo texturizado
	texturedCube.create(&texturedShader);
	texturedCube.load(cubeObj);
	texturedCube.setTexture(shadowMap.getTexture(), 0);
	texturedCube.lock();

	//cria os cubos ECS
	for(int i = 0; i < 10; i++)
	{
		Entity cube = factory->createObject(texturedShaderAsset, cubeObj);
		pipeline->getRegistry()->getComponent<SpatialData>(cube)->set(vec3(-10 + (i*3), 5, 4), vec3(1, 2.4, 2));
		pipeline->getRegistry()->getComponent<CMaterial>(cube)->textures[0] = brickTex->getTexture();
	}

	// Cria o cubo bricks
	{
		normalCube.create(normalShader->getShader());
		normalCube.load(cubeObj);
		normalCube.setTexture(brickTex->getTexture(), 0);
		normalCube.setTexture(brickTexNormal->getTexture(), 1);
		normalCube.setTexture(shadowMap.getTexture(), 2);
		normalCube.lock();

		std::vector<vec3> positions = {};
		vec3 pos;
		for (int y = -10; y < 10; y++)
		{
			for (int x = -10; x < 10; x++)
			{
				pos.x = x * 2.0f;
				pos.y = -1.0f; //+ (float)cos(x) + (float)cos(y);
				pos.z = y * 2.0f;

				positions.push_back(pos);
			}
		}
		normalCube.setInstancesPos(positions);
	}

	//cria a water
	water.create(&waterShader);
	water.load(waterObj);
	water.lock();
	water.set(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	water.setScale(vec3(100, 1, 100));
	{
		float a = 0.0f;
		timerBuffer.create(&a, 1, sizeof(float) * 8);
		timerVertexBuffer.create(&a, 1, sizeof(float) * 8);
	}

	//cria a esfera
	sphere.create(&texturedInstancedShader);
	sphere.load(sphereObj);
	sphere.setTexture(&solidWhiteTex, 0);
	sphere.setTexture(shadowMap.getTexture(), 2);
	sphere.lock();


	//move camera para posicao inicial
	cam.setScreenProportion(resolution3d.y / resolution3d.x);
	cam.setPositionAndAngle({ 0.0f,4.0f,-12.0f }, { 0.0f,0 });

	//CAMERA LUZ
	lightCam.setViewSize(vec2(30, 30));
	lightCam.setPerspective(false);
	lightCam.setPositionAndAngle(vec3(0.f, 18.f, 0.f), vec2(-DirectX::XM_PIDIV2, 0.0f));
	light.setShadowMapProjectionCam(&lightCam);

	//carrega a fonte
	fonte = new Image::font(L"Times New Roman", 40.0f);

	//inicializa as sprites para desenho(hud e target)
	{
		vec2 pos(-1.f, -1.0f);

		const float xFactor = ((eng.getScreenSize().x - 200.0f) / eng.getScreenSize().x);
		vec2 size(xFactor*2.f, 2.0f);
		targetSprite.create(target.getTexture(), pos, size);

		HUD.fromBlank(eng.getScreenSize().x - 200, eng.getScreenSize().y);
		HUDsprite.create(HUD, pos, size);
	}


	//Define gravidade
	phyDomain.setGravity(vec3(0.0f, -10.0f, -0.0f));


	//inicializa guipanel
	gui.create(vec2(eng.getScreenSize().x, eng.getScreenSize().y));
	gui.addValue(L"Frametime", &frameTime);
	gui.addValue(L"FPS", &FPS);
	gui.addValue(L"Quantidade de bolas", &nBolas);
	gui.addValue(L"Posição cubo", &cubePos, false);
	gui.addValue(L"Rotação cubo", &cubeRot, false);
	gui.addValue(L"Altura da luz", &a, false);
	gui.addValue(L"Cubos colidindo ou nao talvez seja mas nao tenho certeza", &cubesColliding);

	

}


void app::input()
{
	//posicao do mouse na tela 
	float x = (float)eng.input().mouseX();
	float y = (float)eng.input().mouseY();

	//codigo para mecher a tela 
	static bool lastRightButtonState = false;
	static vec2 lastMousePos;
	if (eng.input().rightButtonPressed() && lastRightButtonState)
	{
		eng.mouseController().showMouse(false);

		static const float sens = 0.003f;
		float xRaw = (float)eng.input().rawMouseX();
		float yRaw = (float)eng.input().rawMouseY();
		cam.moveAngle({ -yRaw * sens, -xRaw * sens });
		eng.input().resetRaw();


		//win.setMousePosition(win.getWindowSizeX() / 2, win.getWindowSizeY() / 2);
		eng.mouseController().setMousePosition(lastMousePos.x, lastMousePos.y);
	}
	else if (eng.input().rightButtonPressed())
	{
		lastRightButtonState = true;
		eng.input().resetRaw();
		lastMousePos.x = x;
		lastMousePos.y = y;
	}
	else
	{
		eng.mouseController().showMouse(true);
		lastRightButtonState = false;
	}


	//movimento da camera
	auto& kb = eng.input();
	if (kb.isKeyPressed('W')) cam.movePosition({ 0.0f, 0.0f, 0.1f });
	if (kb.isKeyPressed('S')) cam.movePosition({ 0.0f, 0.0f,-0.1f });
	if (kb.isKeyPressed('A')) cam.movePosition({ -0.1f, 0.0f, 0.0f });
	if (kb.isKeyPressed('D')) cam.movePosition({ 0.1f, 0.0f, 0.0f });
	if (kb.isKeyPressed('R')) cam.movePosition({ 0.0f, 0.1f, 0.0f });
	if (kb.isKeyPressed('F')) cam.movePosition({ 0.0f,-0.1f, 0.0f });

	if (kb.isKeyPressed('Z')) a += 0.1f;
	if (kb.isKeyPressed('X')) a -= 0.1f;

	//gui input handling
	bool guiStatus = gui.handleInput(x, y, eng.input().leftButtonPressed());
	if (guiStatus) eng.mouseController().setCursor(CursorController::cursorType::dragHorizontal);
	else eng.mouseController().setCursor(CursorController::cursorType::normal);
}

void app::logic()
{

	//cria mais bolas
	static float lastBallTime = timeSinceCreation.getPassedSeconds();
	nBolas = phyObjs.size();
	if (nBolas < 10 || eng.input().isKeyPressed('C'))
	{
		phyObjs.push_back(new physicsObject(vec3(4 * cos(lastBallTime * 1234.f), 15.0f, 4 * sin(lastBallTime * 78347.f))));
		phyDomain.addObject(phyObjs.back());

		lastBallTime = timeSinceCreation.getPassedSeconds();
	}
	

	//physics solve
	const float pdt = 0.01f;
	while (physicsTime + pdt < timeSinceCreation.getPassedSeconds())
	{
		physicsTime += pdt;
		phyDomain.solve(pdt);
	}

	//testa colisao dos dois cubos
	vec3 move = phyDomain.cubeColliding(
		cubePos - vec3(0.f, 1.f, 0.f), 
		cubeRot,
		vec3(2.f, 1.f, 1.f),
		vec3(1.f, 1.f, 1.f)
	);
	cubePos += move;
	cubesColliding = std::sqrt(move.lengthSquared());
	//draw cube position on hud
	{
		DirectX::XMMATRIX projMat = cam.getMatrix();
		DirectX::XMVECTOR pos = {cubePos.x, cubePos.y, cubePos.z, 1.0f};
		DirectX::XMVECTOR scrPos = DirectX::XMVector4Transform(pos, projMat);
		float w = scrPos.m128_f32[3];
		scrPos = DirectX::XMVectorScale(scrPos, 1.0f / w);
		vec2 vecScrPos(
			scrPos.m128_f32[0],
			scrPos.m128_f32[1]
		);
		vec2 hudRes = HUD.getResolution();
		vecScrPos.y = -vecScrPos.y;
		vecScrPos = (vecScrPos + vec2(1.0f, 1.0f)) / vec2(2.0f,2.0f) * hudRes;
		HUD.drawRectangle(
			vecScrPos,
			vec2(10, 10),
			color(255, 0, 0, 255)
		);
	}

	//atualiza as posições da bola
	std::vector<vec3> ballPositions = {};
	ballPositions.reserve(phyObjs.size());
	for (auto i : phyObjs)
	{
		ballPositions.push_back(i->getPosition());
	}
	sphere.setInstancesPos(ballPositions);


	//muda posicao da luz 
	light.updatePos({ 2.0f + 3, a, 0.0f });
	cubeLight.set({ 2.0f + 3,a,0.0f }, {0.f,0.f,0.f});

	//muda posicao do cubo texturizado
	texturedCube.set(cubePos, cubeRot);

}

void app::draw()
{
	//preenche a tela
	eng.getPipeline()->fillScreen(0.2f, 0.6f, 0.9f);
	target.fill(0.2f, 0.6f, 0.9f);
	targetDS.clear();

	//depth render pass
	shadowMap.clear();
	pipeline->setRenderTarget(nullptr, &shadowMap);
	pipeline->setCamera(&lightCam);
	pipeline->drawObject(sphere);
	pipeline->drawObject(normalCube);
	pipeline->drawObject(texturedCube);


	//forward render pass
	//win.Gfx().drawToScreen();
	pipeline->setRenderTarget(&target, &targetDS);
	pipeline->setCamera(&cam);

	//render normal das esferas
	pipeline->drawObject(sphere);


	//renderiza o chao
	pipeline->drawObject(normalCube);

	//desenha todos os cubos coloridos
	vec3 pos;
	for (int j = -5; j < 5; j++)
	{
		for (int i = -20; i < 20; i++)
		{
			pos.x = i * 4.0f;
			pos.y = cos(timeSinceCreation.getPassedSeconds() * 2 + i) + 3 * 10;
			pos.z = sin(timeSinceCreation.getPassedSeconds() * 2 + i) + j * 10;

			colorBlendCube.set(pos, { 0.f,0.f,0.f });
			pipeline->drawObject(colorBlendCube);
		}
	}

	//desenha a luz
	pipeline->drawObject(cubeLight);


	//coloca o cubo texturizado
	pipeline->drawObject(texturedCube);

	// ECS draw
	pipeline->drawScene();

	//posiciona e renderiza segundo cubo texturizado
	texturedCube.set({ 0.0f,1.0f,0.0f }, { 0.0f,0.0f,0.0f });
	texturedCube.setScale(vec3(2.f, 1.f, 1.f));
	pipeline->drawObject(texturedCube);
	texturedCube.setScale(vec3(1.f, 1.f, 1.f));


	//atualiza timer buffers para a agua
	float time = timeSinceCreation.getPassedSeconds();
	timerBuffer.update(&time);
	timerBuffer.setSlot(2);
	timerBuffer.bind();
	timerVertexBuffer.update(&time);
	timerVertexBuffer.setSlot(3);
	timerVertexBuffer.bind();
	//desenha agua
	pipeline->drawObject(water);

	//escreve texto do frametime
	static float dTime;
	frameTime = (timeSinceCreation.getPassedSeconds() - dTime);
	FPS = 1.0f / frameTime;
	dTime = timeSinceCreation.getPassedSeconds();

	eng.getPipeline()->drawToScreen();

	//desenha e atualiza o hud
	//targetSprite.update(hud);
	targetSprite.draw(*pipeline);

	HUDsprite.update(HUD);
	HUDsprite.draw(*pipeline);
	HUD.clear();
	//hud.clear();

	//desenha o gui
	gui.draw(*pipeline);
}




void app::start()
{


	while (eng.update())
	{
		input();
		logic();
		draw();
	}

}