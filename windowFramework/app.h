#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "colorBlendTriangle.h"

#include "v.h"

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


	colorBlendTriangle cube;

	window win;
	Timer timeSinceCreation;

};