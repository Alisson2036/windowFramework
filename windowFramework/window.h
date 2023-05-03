#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "mouse.h"
#include "keyboard.h"
#include "graphics.h"

class window
{
public:

	window(const LPCWSTR name, int width, int height);
	window(window& w) = delete;
	~window();

	int update();

	HINSTANCE get_hInstance() const;
	Mouse* getMousePointer();
	Keyboard* getKeyboarPointer();
	Graphics& Gfx();

	void setTitle(std::string newTitle);

	void setMousePosition(int x, int y);

	int getWindowSizeX();
	int getWindowSizeY();

private:
	static LRESULT CALLBACK messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT messageHandlerLocal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	int windowWidth;
	int windowHeight;

	HWND hwnd;//window handle
	HINSTANCE hInstance;
	static constexpr const wchar_t* className = L"window class"; //nome da windowClass

	MSG lastMessage;//variavel para guardar a ultima mensagem

	Mouse mouse;
	Keyboard keyboard;
	std::unique_ptr<Graphics> pGfx;
	
};