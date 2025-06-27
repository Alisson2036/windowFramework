#pragma once
#include <Windows.h>

// Controlador de cursor do mouse, responsável por gerenciar o cursor
// e a posição do mouse na janela da aplicação.
class CursorController
{
public:
	explicit CursorController(HWND windowHandle, HCURSOR* cursorOut);

	// Tipos de cursor disponíveis
	enum class cursorType { 
		normal, 
		hand, 
		dragAll, 
		dragVertical, 
		dragHorizontal, 
		wait 
	};

	void setCursor(cursorType cursor);

	void setMousePosition(int x, int y);
	void showMouse(bool show);

private:
	HCURSOR* currentCursor;
	RECT windowRect;
	HWND hwnd;


};