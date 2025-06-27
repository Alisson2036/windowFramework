#pragma once
#include <Windows.h>

// Controlador de cursor do mouse, respons�vel por gerenciar o cursor
// e a posi��o do mouse na janela da aplica��o.
class CursorController
{
public:
	explicit CursorController(HWND windowHandle, HCURSOR* cursorOut);

	// Tipos de cursor dispon�veis
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