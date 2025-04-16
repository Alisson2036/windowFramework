#include "sampler.h"

void Sampler::create(bool isAntialiased)
{
	
	
	D3D11_SAMPLER_DESC sampDesc = {};
	if(isAntialiased)
		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	else
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//linear deixa mais suavizado, para um estilo mais
	//pixel art, usar point

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_throwHr(
		getDevice()->CreateSamplerState(&sampDesc, samplerState.GetAddressOf())
	);
}

void Sampler::bind()
{
	static Sampler* last = nullptr;
	if (last != this)
	{
		last = this;
		getContext()->PSSetSamplers(0u, 1u, samplerState.GetAddressOf());
	}
}
