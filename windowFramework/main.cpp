#include <Windows.h>
#include "window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(11);
	}

	return DefWindowProc(hwnd, msg, WParam, LParam);
}


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	window wnd(L"aa", 800, 600);
	while (wnd.update() > 0);
	return 0;
}