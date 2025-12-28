#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include "..\Input\mouse.h"
#include "..\Input\keyboard.h"
#include "graphics.h"


// Classe responsável por gerenciar a janela da aplicação, incluindo
// criação, atualização e manipulação de eventos de entrada do usuário.
class window
{
public:

	window(const LPCWSTR name, Mouse* _mouse, Keyboard* _keyboard, int width, int height);
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

	void setMousePosition(int x, int y);
	void showMouse(bool show);

	int getWindowSizeX();
	int getWindowSizeY();

	// Variável para armazenar o cursor atual,
	// que pode ser alterada externamente
	HCURSOR currentCursor;

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
	Keyboard* keyboard;
	
};