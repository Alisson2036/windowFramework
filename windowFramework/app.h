#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"
#include "renderTarget.h"
#include "depthStencil.h"

#include "Camera.h"
#include "light.h"

#include "texture.h"

#include "sprite.h"
#include "guiPanel.h"

#include "physicsDomain.h"
#include "physicsObject.h"

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

	Texture tex;
	Texture brickTex;
	Texture brickTexNormal;
	Texture solidWhiteTex;

	float a = 8.0f;
	float frameTime;
	float FPS;

	window win;
	Timer timeSinceCreation;


	sprite targetSprite;
	guiPanel gui;

	shader colorBlendShader;
	shader texturedShader;
	shader normalShader;
	shader waterShader;
	shader texturedInstancedShader;

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

	//teste fisica
	vec3 cubePos = { 1.f,1.f,1.f };
	vec3 cubeRot = { 0.f,0.f,0.f };
};