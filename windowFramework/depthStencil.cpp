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

	//bind depth stencil state na pipeline
	getContext()->OMSetDepthStencilState(depthStencilState.Get(), 1u);

	//criando a textura do depth buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	depthTextureDesc.Width = resolution.x;
	depthTextureDesc.Height = resolution.y;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.MipLevels = 1u;
	depthTextureDesc.ArraySize = 1u;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.SampleDesc.Count = 1u;
	depthTextureDesc.SampleDesc.Quality = 0u;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	_throwHr(getDevice()->CreateTexture2D(&depthTextureDesc, nullptr, depthTexture.GetAddressOf()));


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	_throwHr(getDevice()->CreateDepthStencilView(depthTexture.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()));

	initialized = true;
}

void depthStencil::bind()
{
	getContext()->OMSetDepthStencilState(depthStencilState.Get(), 1u);
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
