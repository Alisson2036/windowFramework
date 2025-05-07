#pragma once
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment( lib, "dxgi.lib" )
#include <windows.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"
#include "bindable.h"
#include "pipeline.h"
#include "image.h"
#include "depthStencil.h"
#include "targetView.h"



class Graphics
{
public:
	
public:
	Graphics(HWND _hwnd, int _windowSizeX, int _windowSizeY);
	Graphics(Graphics&) = delete;
	Graphics operator=(Graphics&) = delete;
	~Graphics();



	//classe que armazena pointers para os objetos
	Pipeline* getPipeline();

	void drawToScreen();
	void drawToTarget(targetView target);
	void fillScreen(float r, float g, float b);
	void flip();


private:
	int windowSizeX;
	int windowSizeY;

	Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>          swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
	depthStencil depthStencilBuffer;

	Pipeline* pipeline;

};

