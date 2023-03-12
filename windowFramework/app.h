#pragma once
#include "window.h"
#include "exception.h"
#include "timer.h"

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