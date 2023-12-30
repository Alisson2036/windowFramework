#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "colorBlendTriangle.h"
#include "texturedTriangle.h"
#include "tex2d.h"
#include "Camera.h"
#include "light.h"

#include "texture.h"

//apenas para teste
#include "shader.h"


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

	colorBlendTriangle cube;
	colorBlendTriangle cubeLight;
	Tex2d plane;

	Texture tex;

	float a = 0.0f;

	window win;
	Timer timeSinceCreation;


	shader colorBlendShader;
	object colorBlendCube;
	shader texturedShader;
	object texturedCube;


	Light light;//precisa ser inicializado depois do win se nao da erro
};