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
	
}

void Graphics::testando()
{
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

	//configurando input layout
	D3D11_INPUT_ELEMENT_DESC ed[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
	d3dDevice->CreateInputLayout(ed, 1, blob->GetBufferPointer(), blob->GetBufferSize(), &il);
	deviceContext->IASetInputLayout(il.Get());


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
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = Triangle;
	sd.SysMemPitch = NULL;
	sd.SysMemSlicePitch = NULL;


	//criando o buffer para armazenar os vertices
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex;
	HRESULT hr = d3dDevice->CreateBuffer(&bd, &sd, vertex.GetAddressOf());
	_throwHr(hr);



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

	

	const UINT stride = sizeof(Vec2);
	const UINT offset = 0;

	//enviando o buffer e tentando desenhar
	deviceContext->IASetVertexBuffers(0, 1, vertex.GetAddressOf(), &stride, &offset);
	deviceContext->Draw(3,0);
}

void Graphics::test2()
{
	// Create Vertex Shader
	ID3DBlob* vsBlob;
	ID3D11VertexShader* vertexShader;
	{
		ID3DBlob* shaderCompileErrorsBlob;
		HRESULT hResult = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, &shaderCompileErrorsBlob);

		hResult = d3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	}

	// Create Pixel Shader
	ID3D11PixelShader* pixelShader;
	{
		ID3DBlob* psBlob;
		ID3DBlob* shaderCompileErrorsBlob;
		HRESULT hResult = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, &shaderCompileErrorsBlob);

		hResult = d3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
		psBlob->Release();
	}

	// Create Input Layout
	ID3D11InputLayout* inputLayout;
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HRESULT hResult = d3dDevice->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		vsBlob->Release();
	}

	// Create Vertex Buffer
	ID3D11Buffer* vertexBuffer;
	UINT numVerts;
	UINT stride;
	UINT offset;
	{
		float vertexData[] = { // x, y, r, g, b, a
			0.0f,  0.5f, 0.f, 1.f, 0.f, 1.f,
			0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
			-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f
		};
		stride = 6 * sizeof(float);
		numVerts = sizeof(vertexData) / stride;
		offset = 0;

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(vertexData);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { vertexData };

		HRESULT hResult = d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	}



	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 700, 500, 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout);

	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->Draw(numVerts, 0);

	swapChain->Present(1, 0);

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
