#include "window.h"

window::window(const LPCWSTR name, int _width, int _height):
	hInstance(GetModuleHandle(NULL)),
	lastMessage(MSG()),
	mouse(),
	keyboard(),
	windowWidth(_width),
	windowHeight(_height)
{
	//criando window class
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
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

	//calculando o tamanho da janela
	RECT windowRect = { 0,0,_width, _height };
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;


	//criando a janela
	hwnd = CreateWindowEx(
		0,
		className,
		name,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		_width,
		_height,
		nullptr,
		nullptr,
		hInstance,
		this
	);

	//mostra a janela
	ShowWindow(hwnd, 1);

	pGfx = std::make_unique<Graphics>(hwnd);

}

window::~window()
{
	UnregisterClass(className, hInstance);
}

int window::update()
{

	//acessa a ultima mensagem da janela e coloca o codigo de retorno em r
	//int r = GetMessage(&lastMessage, nullptr, 0, 0);

	//verifica se ainda tem mensagens para ler
	while (PeekMessage(&lastMessage, nullptr, 0, 0, PM_REMOVE)) {

		//tira a mensagem da fila
		TranslateMessage(&lastMessage);
		DispatchMessage(&lastMessage);

		if (lastMessage.message == WM_QUIT)
		{
			return 0;
		}		
	}

	pGfx->flip();

	return 1;
}


HINSTANCE window::get_hInstance() const
{
	return HINSTANCE();
}

Mouse* window::getMousePointer()
{
	return &mouse;
}

Keyboard* window::getKeyboarPointer()
{
	return &keyboard;
}

Graphics& window::Gfx()
{
	return *pGfx;
}

void window::setTitle(std::string newTitle)
{
	SetWindowTextA(hwnd, newTitle.c_str());
}

int window::getWindowSizeX()
{
	return windowWidth;
}

int window::getWindowSizeY()
{
	return windowHeight;
}

LRESULT CALLBACK window::messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:
		CREATESTRUCT* structP = reinterpret_cast<CREATESTRUCT*>(lParam);
		window* wind = reinterpret_cast<window*>(structP->lpCreateParams);

		wind;
		
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
		break;

	//-------------------------------
	//MOUSE MESSAGES-----------------
	case WM_MOUSEMOVE:
		mouse.updatePosition(MAKEPOINTS(lParam));
		break;
	case WM_LBUTTONDOWN:
		mouse.buttonDown(mouse.leftButton);
		break;
	case WM_LBUTTONUP:
		mouse.buttonUp(mouse.leftButton);
		break;
	case WM_RBUTTONDOWN:
		mouse.buttonDown(mouse.rightButton);
		break;
	case WM_RBUTTONUP:
		mouse.buttonUp(mouse.rightButton);
		break;
	case WM_MOUSEWHEEL:
		mouse.wheelMove(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	//------------------------------------
	//KEYBOARD MESSAGES-------------------
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000)) keyboard.keyDown((char)wParam);
		break;
	case WM_KEYUP:
		keyboard.keyUp((char)wParam);
		break;
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000)) keyboard.keyDown((char)wParam);
		break;
	case WM_SYSKEYUP:
		keyboard.keyUp((char)wParam);
		break;
	//------------------------------------
	//KILL FOCUS MESSAGE------------------
	case WM_KILLFOCUS:
		mouse.emptyButtonList();
		keyboard.emptyKeys();
		break;
	}
	
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
