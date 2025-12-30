#include "Engine.h"

Engine::Engine(UINT screenSizeX, UINT screenSizeY)
	:
	win(L"Game", &mouse, &keyboard, screenSizeX, screenSizeY),
	gfx(win.getWindowHandle(), screenSizeX, screenSizeY),
	pipeline(gfx.getDevice(), 
		gfx.getDeviceContext(), 
		gfx.getBackViewBuffer(),
		&registry,
		&vbCache,
		gfx.getBackDSBuffer(),		
		vec2((float)screenSizeX, (float)screenSizeY)
	),
	inputProxy(&mouse, &keyboard),
	mouseControl(win.getWindowHandle(), &win.currentCursor)
{

	screenSize = vec2((float)screenSizeX, (float)screenSizeY);

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

vec2 Engine::getScreenSize()
{
	return screenSize;
}


CursorController& Engine::mouseController()
{
	return mouseControl;
}

InputProxy& Engine::input()
{
	return inputProxy;
}


Pipeline* Engine::getPipeline()
{
	return &pipeline;
}

Registry* Engine::getRegistry()
{
	return &registry;
}

VertexBufferCache* Engine::getVBCache()
{
	return &vbCache;
}

