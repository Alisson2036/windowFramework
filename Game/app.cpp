#include "app.h"

App::App()
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

	{
		AssetLoader	loader(&assetManager, factory.get());
		loader.loadFromXML("Scene\\scene.xml");
		assetManager.LoadAll();
	}
	// Meshes
	auto* cubeObj = assetManager.getAsset<MeshAsset>("Cube");
	auto* sphereObj = assetManager.getAsset<MeshAsset>("Sphere");
	auto* waterObj = assetManager.getAsset<MeshAsset>("Water");
	auto* coloredCube = assetManager.CreateAsset<MeshAsset>("ColoredCube");
	auto* whiteCube = assetManager.CreateAsset<MeshAsset>("whiteCube");

	// Textures
	auto* tex = assetManager.getAsset<TextureAsset>("a");
	auto* brickTex = assetManager.getAsset<TextureAsset>("Bricks");
	auto* brickTexNormal = assetManager.getAsset<TextureAsset>("BricksNormal");

	// Shaders
	auto* normalShader = assetManager.getAsset<ShaderAsset>("normalShader");
	auto* ecsShader = assetManager.getAsset<ShaderAsset>("ecsShader");
	auto* colorBlendShader = assetManager.getAsset<ShaderAsset>("colorBlendShader");
	auto* texturedShader = assetManager.getAsset<ShaderAsset>("texturedShader");
	auto* texturedInstancedShader = assetManager.getAsset<ShaderAsset>("texturedInstancedShader");
	auto* waterShader = assetManager.getAsset<ShaderAsset>("waterShader");

	// Cria os materiais
	auto* ecsMat = assetManager.getAsset<MaterialAsset>("ecsMat");
	ecsMat->addTexture(shadowMap.getTexture(), 2);

	auto* brickMat = assetManager.getAsset<MaterialAsset>("brickMat");
	brickMat->addTexture(shadowMap.getTexture(), 2);

	auto* coloredMat = assetManager.getAsset<MaterialAsset>("coloredMat");



	// Cria textura de branco solido
	{
		Image solidWhite;
		solidWhite.fromBlank(100, 100);
		solidWhite.drawRectangle(
			vec2(0, 0),
			vec2(100, 100),
			color(255u, 255u, 255u, 255u)
		);
		solidWhiteTex.create(solidWhite);
		auto* texw = assetManager.CreateAsset<TextureAsset>("solidWhiteTex", std::move(solidWhite));
		texw->Load();
		auto* mat = assetManager.CreateAsset<MaterialAsset>("solidWhite", ecsShader, texw);
		mat->addTexture(shadowMap.getTexture(), 2);
	}




	// Load
	assetManager.LoadAll();
	

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
	std::vector<UINT> ind =
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
	coloredCube->manualLoadIndices(ind);
	whiteCube->manualLoad("Position", verArr);
	whiteCube->manualLoad("Color", colorsWhite);
	whiteCube->manualLoadIndices(ind);


	//cria o cubo texturizado
	texturedCube.create(texturedShader->getShader());
	texturedCube.load(cubeObj);
	texturedCube.setTexture(shadowMap.getTexture(), 0);
	texturedCube.lock();

	//cria os cubos ECS
	for(int i = 0; i < 8; i++)
	{
		Entity cube = factory->createObject(
			ecsMat,
			cubeObj,
			SpatialData(vec3(-10.f + (i * 3), 5.f, 4.f), vec3(1.f, 2.4, 2.f))
		);
	}

	// Cria os coloredCubes
	for (int y = -10; y < 10; y++)
	{
		for (int x = -10; x < 10; x++)
		{
			Entity cube = factory->createObject(
				coloredMat,
				coloredCube,
				SpatialData(vec3(x * 4.f, 30.f, y * 6.f), vec3())
			);
		}
	}
	// Cria o light cube
	cubeLight = factory->createObject(
		coloredMat,
		whiteCube,
		SpatialData(vec3(10, 10, 10), vec3())
	);
	pipeline->getRegistry()->getComponent<SpatialData>(cubeLight)->setScale(vec3(.2f,.2f,.2f));



	//cria a water
	water.create(waterShader->getShader());
	water.load(waterObj);
	water.lock();
	water.set(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	water.setScale(vec3(100, 1, 100));
	{
		timerStruct a = {};
		timerBuffer.create(a);
		timerVertexBuffer.create(a);
	}


	//move camera para posicao inicial
	cam.setScreenProportion(resolution3d.y / resolution3d.x);
	cam.setPositionAndAngle({ 0.0f,4.0f,-12.0f }, { 0.0f,0 });

	//CAMERA LUZ
	lightCam.setViewSize(vec2(60, 60));
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
	}


	//Define gravidade
	phyDomain.setGravity(vec3(0.0f, -10.0f, -0.0f));

	// Creating render passes
	{
		// Forward pass
		forwardPass.setCamera(&cam);
		forwardPass.setLight(&light);
		forwardPass.setTargets(&target, &targetDS);

		// ShadowPass
		shadowPass.setLight(&light);
		shadowPass.setTarget(&shadowMap);
	}

	

}


void App::input()
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
	float speed = 30.f * frameTime;
	if (kb.isKeyPressed('W')) cam.movePosition({ 0.0f, 0.0f, speed });
	if (kb.isKeyPressed('S')) cam.movePosition({ 0.0f, 0.0f,-speed });
	if (kb.isKeyPressed('A')) cam.movePosition({-speed, 0.0f, 0.0f });
	if (kb.isKeyPressed('D')) cam.movePosition({ speed, 0.0f, 0.0f });
	if (kb.isKeyPressed('R')) cam.movePosition({ 0.0f, speed, 0.0f });
	if (kb.isKeyPressed('F')) cam.movePosition({ 0.0f,-speed, 0.0f });

	if (kb.isKeyPressed('Z')) a += 0.1f;
	if (kb.isKeyPressed('X')) a -= 0.1f;

	//gui input handling
}

void App::logic()
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


	// Sphere update logic
	struct phyBall { };
	{
		auto view = pipeline->getRegistry()->getView<SpatialData, phyBall>();
		auto iter = view.begin();
		for (auto i : phyObjs)
		{
			SpatialData sd(i->getPosition());
			if (iter == view.end()) {
				Entity e = factory->createObject(
					assetManager.getAsset<MaterialAsset>("solidWhite"),
					assetManager.getAsset<MeshAsset>("Sphere"),
					sd
				);
				pipeline->getRegistry()->addComponent(e, phyBall{});
			}
			else {
				(*iter.get<SpatialData>()) = sd;
				++iter;
			}
		}
	}


	//muda posicao da luz 
	light.updatePos({ 2.0f + 3, a, 0.0f });
	//cubeLight.set({ 2.0f + 3,a,0.0f }, {0.f,0.f,0.f});
	pipeline->getRegistry()->getComponent<SpatialData>(cubeLight)->set(vec3(2.0f + 3, a, 0.0f), vec3());

	//muda posicao do cubo texturizado
	texturedCube.set(cubePos, cubeRot);

}

void App::draw()
{

	{

		// Descobre as dimensões atuais da tela/viewport
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		// Travar a posicao
		const float panelWidth = 200.0f;
		ImGui::SetNextWindowPos(
			ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - panelWidth, viewport->WorkPos.y),
			ImGuiCond_Always
		);

		// Travar o TAMANHO
		ImGui::SetNextWindowSize(
			ImVec2(panelWidth, viewport->WorkSize.y),
			ImGuiCond_Always
		);

		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoTitleBar |            // Remove a barra de título superior
			ImGuiWindowFlags_NoResize |              // Impede o usuário de redimensionar
			ImGuiWindowFlags_NoMove |                // Impede o usuário de arrastar a janela
			ImGuiWindowFlags_NoCollapse |            // Impede de minimizar
			ImGuiWindowFlags_NoSavedSettings;        // Ignora a posição salva no imgui.ini

		ImGui::Begin("Sidebar", nullptr, window_flags);

		ImGui::TextUnformatted("Frametime");
		ImGui::Text("  %.6f", 1.0f / ImGui::GetIO().Framerate);

		ImGui::TextUnformatted("FPS");
		ImGui::Text("  %.1f", ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::TextUnformatted("Quantidade de bolas");
		ImGui::Text("  %d", nBolas);

		ImGui::Separator();

		ImGui::TextUnformatted("Posicao cubo");
		ImGui::DragFloat3("##posCubo", reinterpret_cast<float*>(&cubePos), 0.1f);

		ImGui::TextUnformatted("Rotacao cubo");
		ImGui::DragFloat3("##rotCubo", reinterpret_cast<float*>(&cubeRot), 0.1f);

		ImGui::TextUnformatted("Altura da luz");
		ImGui::SliderFloat("##alturaLuz", &a, 0.0f, 20.0f);


		ImGui::End();
	}


	//preenche a tela
	eng.getPipeline()->fillScreen(0.2f, 0.6f, 0.9f);
	target.fill(0.2f, 0.6f, 0.9f);
	targetDS.clear();

	//depth render pass
	shadowMap.clear();
	pipeline->setRenderTarget(nullptr, &shadowMap);
	pipeline->setCamera(&lightCam);
	pipeline->drawObject(texturedCube);

	// ECS draw
	pipeline->drawScene(std::vector<IRenderPass*>{&shadowPass, & forwardPass});

	//forward render pass
	//win.Gfx().drawToScreen();
	pipeline->setRenderTarget(&target, &targetDS);
	pipeline->setCamera(&cam);
	pipeline->drawObject(texturedCube);


	//posiciona e renderiza segundo cubo texturizado
	texturedCube.set({ 0.0f,1.0f,0.0f }, { 0.0f,0.0f,0.0f });
	texturedCube.setScale(vec3(2.f, 1.f, 1.f));
	pipeline->drawObject(texturedCube);
	texturedCube.setScale(vec3(1.f, 1.f, 1.f));


	//atualiza timer buffers para a agua
	float time = timeSinceCreation.getPassedSeconds();
	timerBuffer.update(timerStruct{time, 0.f});
	timerBuffer.setSlot(2);
	timerBuffer.bind();
	timerVertexBuffer.update(timerStruct{ time, 0.f });
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
	targetSprite.draw(*pipeline);

}




void App::start()
{


	while (eng.update())
	{
		input();
		logic();
		draw();
	}

}