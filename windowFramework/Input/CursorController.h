#pragma once
#include <Windows.h>


class CursorController
{
public:
	explicit CursorController(HWND windowHandle, HCURSOR* cursorOut);

	//supported cursor types
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