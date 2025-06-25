#include "Engine.h"

Engine::Engine(UINT screenSizeX, UINT screenSizeY)
	:
	win(L"Game", screenSizeX, screenSizeY),
	gfx(win.getWindowHandle(), screenSizeX, screenSizeY),
	pipeline(gfx.getDevice(), 
		gfx.getDeviceContext(), 
		gfx.getBackViewBuffer(), 
		gfx.getBackDSBuffer(), 
		vec2(screenSizeX, screenSizeY)
	),
	inputProxy(&mouse, &keyboard),
	mouseControl(win.getWindowHandle(), &win.currentCursor)
{

	screenSize = vec2((float)screenSizeX, (float)screenSizeY);

	//inicia GDI
	Image::initialize();

	//sets mouse and keyboard
	win.setMouse(&mouse);
	win.setKeyboard(&keyboard);


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

