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
	texDesc.MiscFlags = 0;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	//criando a textura vazia
	_throwHr
	(
		getDevice()->CreateTexture2D(&texDesc, nullptr, buffer.texture.GetAddressOf())
	);

	//criando a render target da textura
	_throwHr
	(
		getDevice()->CreateRenderTargetView(
			buffer.texture.Get(),
			nullptr,
			renderTargetView.GetAddressOf()
		)
	);

	//criando a view da textura
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	_throwHr
	(
		getDevice()->CreateShaderResourceView(buffer.texture.Get(), &viewDesc, buffer.textureView.GetAddressOf())
	);

	buffer.resolution = vec2(targetResolution.x, targetResolution.y);



	const float f[4] = { 0.f,0.f,0.f, 1.0f };
	getContext()->ClearRenderTargetView(renderTargetView.Get(), f);

	//retorna caso nao for para usar depthstencil
	if (!enableDepthStencil) return;
	depthStencilBuffer.create(targetResolution);
}

void targetView::bind()
{
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)targetResolution.x, (float)targetResolution.y, 0.0f, 1.0f };
	getContext()->RSSetViewports(1, &viewport);

	if (depthStencilBuffer.isInitialized())
		depthStencilBuffer.bind();
	getContext()->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilBuffer.getViewPointer());

}

Texture* targetView::getTexture()
{
	return &buffer;
}

void targetView::clear()
{
	const float f[4] = { 0.f,0.f,0.f, 1.0f };
	getContext()->ClearRenderTargetView(renderTargetView.Get(), f);

	if (depthStencilBuffer.isInitialized())
		depthStencilBuffer.clear();
}
