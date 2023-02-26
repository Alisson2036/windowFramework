#include "window.h"

window::window(const LPCWSTR name, int width, int height)
{
	//criando window class
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = messageHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"window class";

	//registra a classe
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		0,
		L"window class",
		name,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		GetModuleHandle(NULL),
		nullptr
	);

	//mostra a janela
	ShowWindow(hwnd, 1);

}

window::~window()
{
}

int window::update()
{
	//variavel para armazenar a mensagem
	MSG msg = {};
	//acessa a ultima mensagem e coloca o codigo de retorno em r
	int r = GetMessage(&msg, nullptr, 0, 0);

	//tira a mensagem da fila
	TranslateMessage(&msg);
	DispatchMessage(&msg);

	return r;
}

HINSTANCE window::get_hInstance() const
{
	return HINSTANCE();
}

LRESULT window::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
