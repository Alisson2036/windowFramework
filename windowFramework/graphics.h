#pragma once
#include <windows.h>
#include <d3d11.h>
#pragma comment (lib,"d3d11.lib")
#include "exception.h"

class Graphics
{
public:
	Graphics(HWND _hwnd);
	Graphics(Graphics&) = delete;
	Graphics operator=(Graphics&) = delete;

	~Graphics();

	void fillScreen(float r, float g, float b);

	void flip();

private:

private:
	ID3D11Device* d3dDevice = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11RenderTargetView* targetView = nullptr;

};

