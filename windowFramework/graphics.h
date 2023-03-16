#pragma once
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment( lib, "dxgi.lib" ) 
#include <windows.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"

#include "object.h"

#include "bindable.h"


class Graphics
{
	friend class Bindable;
public:
	
public:
	Graphics(HWND _hwnd);
	Graphics(Graphics&) = delete;
	Graphics operator=(Graphics&) = delete;
	~Graphics();


	/*
	// Draw functions
	*/
	void drawObject(Object obj);

	Object::Fill getFillable();

	void fillScreen(float r, float g, float b);
	void flip();



private:
	/*
	// Getters e Setters
	*/
	Microsoft::WRL::ComPtr<ID3D11Device>         getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>  getContext();

	void getPixelShader(const wchar_t* name);
	void getVertexShader(const wchar_t* name);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>           d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;



};

