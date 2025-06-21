#include "Engine.h"

Engine::Engine(UINT windowSizeX, UINT windowSizeY)
	:
	win(L"Game", windowSizeX, windowSizeY),
	gfx(win.getWindowHandle(), windowSizeX, windowSizeY),
	pipeline(gfx.getDevice(), gfx.getDeviceContext(), vec2(windowSizeX, windowSizeY))
{

	//inicia GDI
	Image::initialize();
}
Engine::~Engine()
{
	Image::uninitialize();
}

bool Engine::update()
{
	bool a = win.update();
	gfx.flip();

	return a;
}

Graphics* Engine::getGfx()
{
	return &gfx;
}

Pipeline* Engine::getPipeline()
{
	return &pipeline;
}

window* Engine::getWindow()
{
	return &win;
}
