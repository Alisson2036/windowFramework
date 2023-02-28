#pragma once
#include <Windows.h>
#include "mouse.h"

class window
{
public:

	window(const LPCWSTR name, int width, int height);
	window(window& w) = delete;
	~window();
	int handleMessage();

	HINSTANCE get_hInstance() const;
	Mouse* getMousePointer();


private:
	static LRESULT CALLBACK messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT messageHandlerLocal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND hwnd;//window handle
	HINSTANCE hInstance;
	static constexpr const wchar_t* className = L"window class"; //nome da windowClass

	MSG lastMessage;//variavel para guardar a ultima mensagem

	Mouse mouse;

	
};