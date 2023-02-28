#include "window.h"

window::window(const LPCWSTR name, int width, int height):
	hInstance(GetModuleHandle(NULL)),
	mouse(Mouse(&lastMessage))
{
	//criando window class
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = messageHandlerSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;

	//registra a classe
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		0,
		className,
		name,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	//mostra a janela
	ShowWindow(hwnd, 1);

}

window::~window()
{
	UnregisterClass(className, hInstance);
}

int window::handleMessage()
{
	
	//acessa a ultima mensagem da janela e coloca o codigo de retorno em r
	int r = GetMessage(&lastMessage, nullptr, 0, 0);

	//tira a mensagem da fila
	TranslateMessage(&lastMessage);
	DispatchMessage(&lastMessage);

	return r;
}


HINSTANCE window::get_hInstance() const
{
	return HINSTANCE();
}

Mouse* window::getMousePointer()
{
	return &mouse;
}

LRESULT CALLBACK window::messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:
		CREATESTRUCT* structP = reinterpret_cast<CREATESTRUCT*>(lParam);
		window* wind = reinterpret_cast<window*>(structP->lpCreateParams);
		
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wind));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wind->messageHandler));
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT window::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//pegando o pointer em GWLP_USERDATA, apontando para a classe criadora da janela, e chamando o messageHandlerLocal
	return reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))->messageHandlerLocal(hwnd, msg, wParam, lParam);
	
}

LRESULT window::messageHandlerLocal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(11);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
