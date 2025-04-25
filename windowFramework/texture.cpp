#include "texture.h"

void Texture::create(Image& img, bool hasAntiAliasing)
{
	antialiased = hasAntiAliasing;

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
	sd.SysMemPitch = d.width * sizeof(color);

	//criando a texture
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

	resolution = vec2(d.width, d.height);

}

void Texture::createWithMipMap(Image& img, bool hasAntiAliasing)
{
	antialiased = hasAntiAliasing;

	Image::data d = img.getData();

	//configurando a texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = d.width;
	texDesc.Height = d.height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//lembrar q bind render target piora otimização, mas é necessário para gerar
	//os mip maps pela gpu
	texDesc.Usage = D3D11_USAGE_DEFAULT;


	//criando a textura, sem a imagem
	_throwHr
	(
		getDevice()->CreateTexture2D(&texDesc, nullptr, texture.GetAddressOf())
	);

	//coloca a imagem no primeiro nivel mip
	getContext()->UpdateSubresource(
		texture.Get(),
		0u,
		nullptr,
		d.data,
		d.width * sizeof(color),
		0u
	);
	

	//criando a view da textura
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = -1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	_throwHr
	(
		getDevice()->CreateShaderResourceView(texture.Get(), &viewDesc, textureView.GetAddressOf())
	);

	//cria o MipMap
	getContext()->GenerateMips(textureView.Get());

	resolution = vec2(d.width, d.height);
}

void Texture::update(Image& img)
{
	Image::data d = img.getData();

	resolution = vec2(d.width, d.height);

	//coloca a imagem no primeiro nivel mip
	getContext()->UpdateSubresource(
		texture.Get(),
		0u,
		nullptr,
		d.data,
		d.width * sizeof(color),
		0u
	);
}

bool Texture::isAntialiased()
{
	return antialiased;
}

void Texture::setSlot(unsigned short slot)
{
	textureSlot = slot;
}

void Texture::bind()
{

	static Texture* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->PSSetShaderResources(textureSlot, 1u, textureView.GetAddressOf());
	}
}

vec2 Texture::getResolution()
{
	return resolution;
}
