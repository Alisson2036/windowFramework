#include "renderTarget.h"

void renderTarget::create(vec2 targetSize)
{

	//criando a textura do buffer
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = (UINT)targetSize.x;
	texDesc.Height = (UINT)targetSize.y;
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
		getDevice()->CreateTexture2D(&texDesc, nullptr, m_texture.GetAddressOf())
	);

	create_internal(texDesc);
}

void renderTarget::create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
{
	D3D11_TEXTURE2D_DESC texDesc = {};
	texture->GetDesc(&texDesc);

	m_texture = texture;

	create_internal(texDesc);
}

ID3D11RenderTargetView** renderTarget::getViewPointer()
{
	return renderTargetView.GetAddressOf();
}

void renderTarget::bind()
{
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)targetResolution.x, (float)targetResolution.y, 0.0f, 1.0f };
	getContext()->RSSetViewports(1, &viewport);


}

void renderTarget::create_internal(D3D11_TEXTURE2D_DESC texDesc)
{

	targetResolution = vec2(texDesc.Width, texDesc.Height);

	//criando a render target da textura
	_throwHr
	(
		getDevice()->CreateRenderTargetView(
			m_texture.Get(),
			nullptr,
			renderTargetView.GetAddressOf()
		)
	);

	// Creates texture view if it can be bound to a shader
	if(texDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = 1;
		viewDesc.Texture2D.MostDetailedMip = 0;

		_throwHr
		(
			getDevice()->CreateShaderResourceView(m_texture.Get(), &viewDesc, textureView.GetAddressOf())
		);

		// Creates texture
		texInterface.create(m_texture.Get(), textureView.Get());
	}


	const float f[4] = { 0.f,0.f,0.f, 1.0f };
	getContext()->ClearRenderTargetView(renderTargetView.Get(), f);


}

Texture* renderTarget::getTexture()
{
	if(texInterface.isInitialized())
		return &texInterface;
	return nullptr;
}

vec2 renderTarget::getResolution()
{
	return targetResolution;
}

void renderTarget::clear()
{
	const float f[4] = { 0.f,0.f,0.f, 1.0f };
	getContext()->ClearRenderTargetView(renderTargetView.Get(), f);

}

void renderTarget::fill(float r, float g, float b)
{
	const float f[4] = { r, g, b, 1.0f };
	getContext()->ClearRenderTargetView(renderTargetView.Get(), f);
}
