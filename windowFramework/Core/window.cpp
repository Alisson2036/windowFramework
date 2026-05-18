#include "window.h"

Window::Window(const LPCWSTR name, Mouse* _mouse, Keyboard* _keyboard, int _width, int _height):
	hInstance(GetModuleHandle(NULL)),
	lastMessage(MSG()),
	windowWidth(_width),
	windowHeight(_height),
	mouse(_mouse),
	keyboard(_keyboard)
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
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;

	//registra a classe
	RegisterClassEx(&wc);

	//calculando o tamanho da janela
	windowRect = { 0,0,_width, _height };
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
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


	// ativa raw mouse input
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if(
		RegisterRawInputDevices(&rid, 1u, sizeof(rid)) == FALSE
		)
		throw;

	//carrega o cursor padrao
	currentCursor = LoadCursor(NULL, IDC_ARROW);

}

Window::~Window()
{
	UnregisterClass(className, hInstance);
	//DestroyWindow(hwnd);
}

int Window::update()
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

	return 1;
}


HINSTANCE Window::get_hInstance() const
{
	return HINSTANCE();
}

Mouse* Window::getMousePointer()
{
	return mouse;
}

Keyboard* Window::getKeyboarPointer()
{
	return keyboard;
}

HWND Window::getWindowHandle() const
{
	return hwnd;
}


void Window::setTitle(const std::string& newTitle)
{
	SetWindowTextA(hwnd, newTitle.c_str());
}

void Window::setMouse(Mouse* pMouse)
{
	mouse = pMouse;
}

void Window::setKeyboard(Keyboard* pKeyboard)
{
	keyboard = pKeyboard;
}

void Window::setMousePosition(int x, int y)
{
	LPRECT rect = new RECT();
	GetWindowRect(hwnd, rect);
	if (rect)
	{
		SetCursorPos(x + rect->left - windowRect.left, y + rect->top - windowRect.top);
		delete rect;
	}

}

void Window::showMouse(bool show)
{
	CURSORINFO ci = { sizeof(CURSORINFO) };
	GetCursorInfo(&ci);
	if(ci.flags != show)
		ShowCursor(show);
}


int Window::getWindowSizeX()
{
	return windowWidth;
}

int Window::getWindowSizeY()
{
	return windowHeight;
}

LRESULT CALLBACK Window::messageHandlerSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:
		CREATESTRUCT* structP = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* wind = reinterpret_cast<Window*>(structP->lpCreateParams);

		wind;
		
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wind));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wind->messageHandler));

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//pegando o pointer em GWLP_USERDATA, apontando para a classe criadora da janela, e chamando o messageHandlerLocal
	return reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))->messageHandlerLocal(hwnd, msg, wParam, lParam);
	
}

LRESULT Window::messageHandlerLocal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(11);
		break;

	//-------------------------------
	//MOUSE MESSAGES-----------------
	case WM_MOUSEMOVE:
		mouse->updatePosition(MAKEPOINTS(lParam));
		break;
	case WM_LBUTTONDOWN:
		mouse->buttonDown(mouse->leftButton);
		break;
	case WM_LBUTTONUP:
		mouse->buttonUp(mouse->leftButton);
		break;
	case WM_RBUTTONDOWN:
		mouse->buttonDown(mouse->rightButton);
		break;
	case WM_RBUTTONUP:
		mouse->buttonUp(mouse->rightButton);
		break;
	case WM_MOUSEWHEEL:
		mouse->wheelMove(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT) {
            SetCursor(currentCursor);
            return TRUE;
        }
		break;
	//-------------------------------
	//RAW MOUSE MESSAGES-------------
	case WM_INPUT:
	{
		UINT size{};
		UINT out{};
		//descobre o tamanho do buffer
		out = GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)
		);
		if (out == -1) break;

		//le o buffer
		rawBuffer.resize(size);
		out = GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)
		);
		if (out == -1) break;

		//salva as informacoes
		const RAWINPUT input = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (input.header.dwType == RIM_TYPEMOUSE &&
			(input.data.mouse.lLastX != 0 || input.data.mouse.lLastY != 0)
			)
		{
			mouse->updateRawPosition(input.data.mouse.lLastX, input.data.mouse.lLastY);
		}
		break;
	}
	//------------------------------------
	//KEYBOARD MESSAGES-------------------
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000)) keyboard->keyDown((char)wParam);
		break;
	case WM_KEYUP:
		keyboard->keyUp((char)wParam);
		break;
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000)) keyboard->keyDown((char)wParam);
		break;
	case WM_SYSKEYUP:
		keyboard->keyUp((char)wParam);
		break;
	//------------------------------------
	//KILL FOCUS MESSAGE------------------
	case WM_KILLFOCUS:
		mouse->emptyButtonList();
		keyboard->emptyKeys();
		break;
	case WM_NCMOUSELEAVE:
		mouse->emptyButtonList();
		break;
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
