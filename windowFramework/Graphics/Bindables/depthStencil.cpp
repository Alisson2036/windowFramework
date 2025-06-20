#include "depthStencil.h"

void depthStencil::create(vec2 bufferResolution)
{
	resolution = bufferResolution;

	//configura depth buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	_throwHr(getDevice()->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf()));


	//criando a textura do depth buffer
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	depthTextureDesc.Width = resolution.x;
	depthTextureDesc.Height = resolution.y;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.MipLevels = 1u;
	depthTextureDesc.ArraySize = 1u;
	depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTextureDesc.SampleDesc.Count = 1u;
	depthTextureDesc.SampleDesc.Quality = 0u;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	_throwHr(getDevice()->CreateTexture2D(&depthTextureDesc, nullptr, pTexture.GetAddressOf()));

	//criando a view da textura
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	_throwHr
	(
		getDevice()->CreateShaderResourceView(pTexture.Get(), &viewDesc, textureView.GetAddressOf())
	);

	//criando DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;
	_throwHr(getDevice()->CreateDepthStencilView(pTexture.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()));

	//criando a textura
	texInterface.create(pTexture.Get(), textureView.Get());

	initialized = true;
}

void depthStencil::bind()
{
	getContext()->OMSetDepthStencilState(depthStencilState.Get(), 1u);
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)resolution.x, (float)resolution.y, 0.0f, 1.0f };
	getContext()->RSSetViewports(1, &viewport);
}

void depthStencil::clear()
{
	if (!initialized) _throwMsg("Depth Stencil has not been initialized.");

	getContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

ID3D11DepthStencilView* depthStencil::getViewPointer()
{
	if (initialized)
		return depthStencilView.Get();
	return nullptr;
}

Texture* depthStencil::getTexture()
{
	return &texInterface;
}

vec2 depthStencil::getResolution()
{
	return resolution;
}
