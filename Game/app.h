#pragma once
#include "Engine.h"
#include "Core\exception.h"
#include "Utils\timer.h"
#include "Graphics\Bindables\renderTarget.h"
#include "Graphics\Bindables\depthStencil.h"

#include "Assets\AssetManager.h"
#include "Assets\MeshAsset.h"
#include "Assets\ShaderAsset.h"
#include "Assets\TextureAsset.h"

#include "Graphics\Camera.h"
#include "Graphics\light.h"

#include "Graphics\Bindables\texture.h"

#include "Graphics\sprite.h"
#include "UI\guiPanel.h"

#include "Physics\physicsDomain.h"
#include "Physics\physicsObject.h"

#include "Graphics\Shader\ShaderCompiler.h"

#include "ECS\Registry.h"

class app
{
public:
	app();
	app(app&) = delete;
	app operator=(app&) = delete;

	void start();

private:

	void input();
	void logic();
	void draw();


private:
	Pipeline* pipeline;
	Camera cam;
	Camera lightCam;

	Texture solidWhiteTex;

	float a = 8.0f;
	float frameTime;
	float FPS;

	Engine eng;
	Timer timeSinceCreation;


	sprite targetSprite;
	guiPanel gui;

	Image HUD;
	sprite HUDsprite;

	Shader colorBlendShader;
	Shader texturedShader;
	Shader normalShader;
	Shader waterShader;
	Shader texturedInstancedShader;

	object colorBlendCube;
	object texturedCube;
	object normalCube;
	object cubeLight;
	object sphere;

	object water;
	ConstantPixelBuffer timerBuffer;
	ConstantVertexBuffer timerVertexBuffer;

	Image::font* fonte;

	//render target para a area 3d
	renderTarget target;
	depthStencil targetDS;
	vec2 resolution3d;


	depthStencil shadowMap;

	Light light;//precisa ser inicializado depois do win se nao da erro

	//physics objects
	physicsDomain phyDomain;
	float physicsTime;
	std::vector<physicsObject*> phyObjs;
	int nBolas = 0;
	float cubesColliding = 0;

	AssetManager assetManager;

	//teste fisica
	vec3 cubePos = { 3.f,2.f,3.f };
	vec3 cubeRot = { -1.4f,-1.6f,-1.f };
};