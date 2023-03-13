#pragma once
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment( lib, "dxgi.lib" ) 
#include <windows.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <d3d11.h>
#include "exception.h"

class Graphics
{
public:
	Graphics(HWND _hwnd);
	Graphics(Graphics&) = delete;
	Graphics operator=(Graphics&) = delete;

	void testando();
	void test2();

	~Graphics();

	void fillScreen(float r, float g, float b);

	void flip();

private:

	void getPixelShader(const wchar_t* name);
	void getVertexShader(const wchar_t* name);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>           d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;

	Microsoft::WRL::ComPtr<ID3D11PixelShader>   pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob>            pixelShaderBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>  vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob>            vertexShaderBlob;

};

