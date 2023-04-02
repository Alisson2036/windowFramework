#include "texture.h"

void Texture::create(Image& img)
{
	Image::data d = img.getData();

	//configurando a texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = d.width;
	texDesc.Height = d.height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = d.data;
	sd.SysMemPitch = d.width * sizeof(Image::pixel);

	//criando a texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	_throwHr
	(
		getDevice()->CreateTexture2D(&texDesc, &sd, texture.GetAddressOf())
	);



	//criando a view da textura
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	_throwHr
	(
		getDevice()->CreateShaderResourceView(texture.Get(), &viewDesc, textureView.GetAddressOf())
	);



}

void Texture::bind()
{

	getContext()->PSSetShaderResources(0u, 1u, textureView.GetAddressOf());

}
