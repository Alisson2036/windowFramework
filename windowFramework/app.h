#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "Camera.h"
#include "light.h"

#include "texture.h"

#include "hudElement.h"


class app
{
public:
	app();
	app(app&) = delete;
	app operator=(app&) = delete;

	void start();

private:

	void loop();


private:
	Camera cam;

	Texture tex;
	Texture brickTex;
	Texture brickTexNormal;

	float a = 8.0f;

	window win;
	Timer timeSinceCreation;


	hudElement hudObject;

	shader colorBlendShader;
	shader texturedShader;
	shader normalShader;

	object colorBlendCube;
	object texturedCube;
	object normalCube;
	object cubeLight;
	object sphere;

	Image hud;
	Image::font* fonte;

	Light light;//precisa ser inicializado depois do win se nao da erro
};