#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "..\Input\mouse.h"
#include "..\Input\keyboard.h"
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
	HWND getWindowHandle() const;

	void setTitle(const std::string& newTitle);

	void setMouse(Mouse* pMouse);
	void setKeyboard(Keyboard* pKeyboard);

	enum class cursorType { normal, hand, dragAll, dragVertical, dragHorizontal, wait};
	void setMousePosition(int x, int y);
	void showMouse(bool show);
	void setCursor(cursorType cursor);

	int getWindowSizeX();
	int getWindowSizeY();

private:
	static LRESULT CALLBACK messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT messageHandlerLocal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	int windowWidth;
	int windowHeight;
	//RECT que armazena tamanho da janela em relacao ao client region
	RECT windowRect;

	//buffer para raw input
	std::vector<char> rawBuffer;

	HWND hwnd;//window handle
	HINSTANCE hInstance;
	static constexpr const wchar_t* className = L"window class"; //nome da windowClass

	MSG lastMessage;//variavel para guardar a ultima mensagem

	Mouse* mouse;
	HCURSOR currentCursor;
	Keyboard* keyboard;
	
};