#pragma once
#include "core\window.h"
#include "Core\graphics.h"
#include "Graphics\pipeline.h"

class Engine
{
public:
	Engine(UINT windowSizeX, UINT windowSizeY);
	~Engine();

	bool update();

	Graphics* getGfx();
	Pipeline* getPipeline();
	window* getWindow();

private:
	window win;
	Graphics gfx;
	Pipeline pipeline;
};