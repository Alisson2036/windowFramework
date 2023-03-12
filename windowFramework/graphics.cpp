#include "graphics.h"

Graphics::Graphics(HWND hWnd)
{
	DXGI_MODE_DESC modeDesc = {};
	modeDesc.Width = 0; 
	modeDesc.Height = 0; //adquire valores da janela de saída
	modeDesc.RefreshRate = {0,1}; //valor padrao
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
	

	Microsoft::WRL::ComPtr<ID3D11Texture2D> buff;
	swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), &buff);
	d3dDevice->CreateRenderTargetView(buff.Get(), nullptr, &targetView);
	/*
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	_throwHr(hr);
	if (pBackBuffer != nullptr) d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &targetView);
	else _throw;
	*/
}

void Graphics::testando()
{
	//strutura para cada ponto
	struct Vec2
	{
		float x;
		float y;
	};
	//posicoes dos vertices do triangulo
	const Vec2 Triangle[] =
	{
		{ 0.0f, 0.5f },
		{ 0.5f, 0.5f },
		{-0.5f,-0.5f },
	};

	//informacoes sobre o buffer
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Triangle);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(Vec2);

	//buffer para ser mandado para GPU, contendo informacoes dos vertices do triangulo
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = Triangle;


	//criando o buffer para armazenar os vertices
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex;
	HRESULT hr = d3dDevice->CreateBuffer(&bd, &sd, &vertex);
	_throwHr(hr);


	//criando o pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	_throwHr(D3DReadFileToBlob(L"PixelShader.cso", blob.GetAddressOf()));
	d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps);

	//enviando pixel shader
	deviceContext->PSSetShader(ps.Get(), nullptr, 0);


	//criando o vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	_throwHr(D3DReadFileToBlob(L"VertexShader.cso", blob.GetAddressOf()));
	d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs);
	
	//enviando o vertex shader
	deviceContext->VSSetShader(vs.Get(), nullptr, 0u);


	//configurando viewport
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	deviceContext->RSSetViewports(1, &vp);

	//set render target
	deviceContext->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);


	//configurando topologia
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//configurando input layout
	D3D11_INPUT_ELEMENT_DESC ed[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
	d3dDevice->CreateInputLayout(ed, 1, blob->GetBufferPointer(), blob->GetBufferSize(), &il);
	deviceContext->IASetInputLayout(il.Get());

	const UINT stride = sizeof(Vec2);
	const UINT offset = 0;

	//enviando o buffer e tentando desenhar
	deviceContext->IASetVertexBuffers(0, 1, vertex.GetAddressOf(), &stride, &offset);
	deviceContext->Draw(3,0);
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
