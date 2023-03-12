#pragma once
#pragma comment (lib,"d3d11.lib")
#include <windows.h>
#include <wrl.h>
#include <d3d11.h>
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
	Microsoft::WRL::ComPtr<ID3D11Device>           d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;

};

