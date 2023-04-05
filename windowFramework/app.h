#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "colorBlendTriangle.h"
#include "texturedTriangle.h"
#include "Camera.h"
#include "light.h"

#include "texture.h"


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
	TexturedTriangle cubeTex;

	Texture tex;

	float a = 0.0f;

	window win;
	Timer timeSinceCreation;


	Light light;//precisa ser inicializado depois do win se nao da erro
};