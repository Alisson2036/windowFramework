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
	swapChainDesc.BufferCount = 2; //1 back buffer, 1 front buffer, aumentar se precisar de mais 
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE; mudar talvez, nao sei
	
	
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, //usa o feature level array padrão
		1, //numero de elementos no array do feature level
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&d3dDevice,
		NULL,
		&deviceContext
	);
	_throwHr(hr);
	

	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
	_throwHr(hr);
	if (pBackBuffer != nullptr) d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, targetView.GetAddressOf());
	else _throw;


	//configurando os shaders
	getVertexShader(L"VertexShader.cso");
	getPixelShader(L"PixelShader.cso");
	
}


void Graphics::test2()
{
	//Criando input layout
	
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	HRESULT hResult = d3dDevice->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);


	// Create Vertex Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	UINT numVerts;
	UINT stride;
	UINT offset;
	{
		float a[] = { // x, y, r, g, b, a
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Graphics::vertex2d vertexData[3] =
		{
			Graphics::vertex2d(0.0f, 0.5f, 255,0,0,255),
			Graphics::vertex2d(0.5f,-0.5f, 255,0,0,255),
			Graphics::vertex2d(-0.5f,-0.5f, 255,0,0,255),
		};
		stride = sizeof(vertex2d);
		numVerts = 3;
		offset = 0;

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(vertexData);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { vertexData };

		HRESULT hResult = d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	}



	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 800, 600, 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout.Get());

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	deviceContext->Draw(numVerts, 0);


}

void Graphics::draw2dTriangle(vertex2d vertices[3])
{
	//Criando input layout

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	_throwHr(d3dDevice->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout));


	// Create Vertex Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	UINT numVerts;
	UINT stride;
	UINT offset;
	{
		stride = sizeof(vertex2d);
		numVerts = 3;
		offset = 0;

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = stride*3;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { vertices };

		_throwHr(d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer));
	}



	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 800, 600, 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout.Get());

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	deviceContext->Draw(numVerts, 0);
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

void Graphics::getPixelShader(const wchar_t* name)
{
	//criando o pixel shader
	_throwHr(D3DReadFileToBlob(name, pixelShaderBlob.GetAddressOf()));
	_throwHr(d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader));
	
}

void Graphics::getVertexShader(const wchar_t* name)
{
	//criando o vertex shader
	_throwHr(D3DReadFileToBlob(name, vertexShaderBlob.GetAddressOf()));
	d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
}
