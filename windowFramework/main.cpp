#include <Windows.h>


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"window class";

	RegisterClassEx(&wc);


	HWND hwnd = CreateWindowEx(
		0,
		L"window class",
		L"janela",
		0,
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
	
	return 0;
}