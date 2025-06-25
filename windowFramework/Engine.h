#pragma once
#include "Core\window.h"
#include "Core\graphics.h"
#include "Graphics\pipeline.h"
#include "Input\keyboard.h"
#include "Input\mouse.h"
#include "Input\CursorController.h"
#include "Input\inputProxy.h"

class Engine
{
public:
	Engine(UINT screenSizeX, UINT screenSizeY);
	~Engine();

	bool update();

	vec2 getScreenSize();

	CursorController& mouseController();
	InputProxy& input();
	Pipeline* getPipeline();

private:
	vec2 screenSize;
	window win;
	Graphics gfx;
	Pipeline pipeline;

	InputProxy inputProxy;
	CursorController mouseControl;
	Keyboard keyboard;
	Mouse mouse;
};