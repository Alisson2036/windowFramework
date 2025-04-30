#include "targetView.h"

void targetView::create(vec2 targetSize, bool enableDepthStencil)
{
	targetResolution = targetSize;

	//criando a textura do buffer
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = targetResolution.x;
	texDesc.Height = targetResolution.y;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	//criando a textura vazia
	_throwHr
	(
		getDevice()->CreateTexture2D(&texDesc, nullptr, buffer.texture.GetAddressOf())
	);

	//criando a view da textura
	getDevice()->CreateRenderTargetView(
		buffer.texture.Get(),
		nullptr,
		renderTargetView.GetAddressOf()
	);

	//retorna caso nao for para usar depthstencil
	if (!enableDepthStencil) return;

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
	depthTextureDesc.Width = targetSize.x;
	depthTextureDesc.Height = targetSize.y;
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

}

void targetView::bind()
{
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)targetResolution.x, (float)targetResolution.y, 0.0f, 1.0f };
	getContext()->RSSetViewports(1, &viewport);

	getContext()->OMSetDepthStencilState(depthStencilState.Get(), 1u);
}
