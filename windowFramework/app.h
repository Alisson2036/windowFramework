#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "colorBlendTriangle.h"
#include "texturedTriangle.h"
#include "Camera.h"

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
	TexturedTriangle cubeTex;

	Texture tex;

	window win;
	Timer timeSinceCreation;

};