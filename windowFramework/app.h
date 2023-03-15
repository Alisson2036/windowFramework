#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

#include "triangle.h"

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

	window win;
	Timer timeSinceCreation;

};