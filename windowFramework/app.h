#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

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

	Texture tex;

	float a = 0.0f;

	window win;
	Timer timeSinceCreation;


	shader colorBlendShader;
	shader texturedShader;
	object colorBlendCube;
	object texturedCube;
	object cubeLight;


	Light light;//precisa ser inicializado depois do win se nao da erro
};