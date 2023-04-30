#include "sampler.h"

void Sampler::create()
{
	
	
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//linear deixa mais suavizado, para um estilo mais
	//pixel art, usar point

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

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
