#include <Windows.h>


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
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	//criando window class
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"window class";

	RegisterClassEx(&wc);

	//criando a janela
	HWND hwnd = CreateWindowEx(
		0,
		L"window class",
		L"janela",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0,
		0,
		800,
		600,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hwnd, nCmdShow);
	MSG msg = {};
	
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}