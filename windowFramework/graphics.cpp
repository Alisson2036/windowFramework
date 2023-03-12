#include "graphics.h"

Graphics::Graphics(HWND hWnd)
{
	DXGI_MODE_DESC modeDesc = {};
	modeDesc.Width = 0; 
	modeDesc.Height = 0; //adquire valores da janela de saída
	modeDesc.RefreshRate = {0,0}; //valor padrao
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;



	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1; //1 back buffer, 1 front buffer, aumentar se precisar de mais 
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE; mudar talvez, nao sei
	

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL, //usa o feature level array padrão
		0, //numero de elementos no array do feature level
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&d3dDevice,
		NULL,
		&deviceContext
	);
	_throwHr(hr);
	

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	_throwHr(hr);
	if (pBackBuffer != nullptr) d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &targetView);
	else _throw;

}

Graphics::~Graphics()
{
}

void Graphics::fillScreen(float r, float g, float b)
{
	const float f[4] = { r, g, b, 1.0f};
	deviceContext->ClearRenderTargetView(targetView.Get(), f);
}

void Graphics::flip()
{
	
	swapChain->Present(1, 0);
}
