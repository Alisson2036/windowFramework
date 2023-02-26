#pragma once
#include <Windows.h>

class window
{
public:
	//contrutores
	window(const LPCWSTR name, int width, int height);
	~window();

	//funcoes
	int update();

	//getters e setters
	HINSTANCE get_hInstance() const;

	
private:
	LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND hwnd;//window handle
	
};