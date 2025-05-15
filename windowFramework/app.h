#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"
#include "renderTarget.h"
#include "depthStencil.h"

#include "Camera.h"
#include "light.h"

#include "texture.h"

#include "hudElement.h"

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

	window win;
	Timer timeSinceCreation;


	hudElement hudObject;

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

	Image hud;
	Image::font* fonte;

	//render target secundario para o cubo
	renderTarget newTarget;
	depthStencil newDTTarget;

	Light light;//precisa ser inicializado depois do win se nao da erro

	//physics objects
	physicsDomain phyDomain;
	float physicsTime;
	std::vector<physicsObject*> phyObjs;
};